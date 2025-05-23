// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemLibrary.h"
#include "BaseGameplayTags.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "Interaction/CombatInterface.h"

struct AuraDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage)
	
    AuraDamageStatics()
    {
	    DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, Armor, Target, false);
	    DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, BlockChance, Target, false);
    	DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, ArmorPenetration,Source,false);
    	DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, CriticalHitChance,Source,false);
    	DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, CriticalHitResistance,Target,false);
    	DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, CriticalHitDamage,Source,false);
    }
	
};

static const AuraDamageStatics& DamageStatics()
{
	static AuraDamageStatics DStatics;
	return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC=ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC=ExecutionParams.GetTargetAbilitySystemComponent();

    AActor* SourceAvatar=SourceASC->GetAvatarActor();
    AActor* TargetAvatar=TargetASC->GetAvatarActor();

	ICombatInterface* SourceCombatInterface=Cast<ICombatInterface>(SourceAvatar);
	ICombatInterface* TargetCombatInterface=Cast<ICombatInterface>(TargetAvatar);

	const FGameplayEffectSpec& EffectSpec=ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags=EffectSpec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags=EffectSpec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluatedParams;
	EvaluatedParams.SourceTags=SourceTags;
	EvaluatedParams.TargetTags=TargetTags;

	
    // Get Damage Set by Caller Magnitude
	float Damage=EffectSpec.GetSetByCallerMagnitude(FBaseGameplayTags::Get().Damage);
	
    float TargetBlockChance=0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef,EvaluatedParams,TargetBlockChance);
	TargetBlockChance=FMath::Max<float>(TargetBlockChance,0.f);

	float TargetArmor=0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef,EvaluatedParams,TargetArmor);
	TargetArmor=FMath::Max<float>(TargetArmor,0.f);

	float SourceArmorPenetration=0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef,EvaluatedParams,SourceArmorPenetration);
	SourceArmorPenetration=FMath::Max<float>(SourceArmorPenetration,0.f);

	float SourceCriticalHitChance=0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef,EvaluatedParams,SourceCriticalHitChance);
	SourceCriticalHitChance=FMath::Max<float>(SourceCriticalHitChance,0.f);

	float TargetCriticalHitResistance=0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitResistanceDef,EvaluatedParams,TargetCriticalHitResistance);
	TargetCriticalHitResistance=FMath::Max<float>(TargetCriticalHitResistance,0.f);

	float SourceCriticalHitDamage=0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef,EvaluatedParams,SourceCriticalHitDamage);
	SourceCriticalHitDamage=FMath::Max<float>(SourceCriticalHitDamage,0.f);
	
	//if block, cut to damage half
	const bool bBlocked=FMath::RandRange(0.f,100.f)<TargetBlockChance;
	Damage=bBlocked ? Damage/2.f : Damage;

	UCharacterClassInfo* ClassInfo=UAuraAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);
	FRealCurve* ArmorPenetrationCurve=ClassInfo->DamageCalculationCoefficient->FindCurve(FName("ArmorPenetration"),FString());
	float ArmorPenetrationCoefficient=ArmorPenetrationCurve->Eval(SourceCombatInterface->GetLevel());
	
	// ArmorPenetration ignores a percentage of the Target's Armor.	
	const float EffectiveArmor = TargetArmor * ( 100 - SourceArmorPenetration * ArmorPenetrationCoefficient ) / 100.f;

	FRealCurve* EffectiveArmorCurve=ClassInfo->DamageCalculationCoefficient->FindCurve(FName("EffectiveArmor"),FString());
	float EffectiveArmorCoefficient=EffectiveArmorCurve->Eval(TargetCombatInterface->GetLevel());
	
	// Armor ignores a percentage of incoming Damage.
	Damage =Damage* ( 100 - EffectiveArmor * EffectiveArmorCoefficient ) / 100.f;

	FRealCurve* CriticalHitResistanceCurve=ClassInfo->DamageCalculationCoefficient->FindCurve(FName("CriticalHitResistance"),FString());
	float CriticalHitResistanceCoefficient = CriticalHitResistanceCurve->Eval(TargetCombatInterface->GetLevel());

	const float EffectiveCriticalHitChance = SourceCriticalHitChance - TargetCriticalHitResistance * CriticalHitResistanceCoefficient;
	const bool bCriticalDamage=FMath::RandRange(0.f,100.f)<EffectiveCriticalHitChance;
	Damage=bCriticalDamage ? Damage*2.f+SourceCriticalHitDamage : Damage;
	
	const FGameplayModifierEvaluatedData EvaluatedData(UBaseAttributeSet::GetIncomingDamageAttribute(),EGameplayModOp::Additive,Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
