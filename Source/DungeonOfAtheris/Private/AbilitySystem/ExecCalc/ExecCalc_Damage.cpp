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
	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance);

	TMap<FGameplayTag,FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;
	
    AuraDamageStatics()
    {
	    DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, Armor, Target, false);
	    DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, BlockChance, Target, false);
    	DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, ArmorPenetration,Source,false);
    	DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, CriticalHitChance,Source,false);
    	DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, CriticalHitResistance,Target,false);
    	DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, CriticalHitDamage,Source,false);
    	DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, FireResistance,Target,false);
    	DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, ArcaneResistance,Target,false);
    	DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, LightningResistance,Target,false);
    	DEFINE_ATTRIBUTE_CAPTUREDEF(UBaseAttributeSet, PhysicalResistance,Target,false);

    	FBaseGameplayTags Tags=FBaseGameplayTags::Get();

    	TagsToCaptureDefs.Add(Tags.Attribute_Secondary_Armor,ArmorDef);
    	TagsToCaptureDefs.Add(Tags.Attribute_Secondary_BlockChance,BlockChanceDef);
    	TagsToCaptureDefs.Add(Tags.Attribute_Secondary_ArmorPenetration,ArmorPenetrationDef);
    	TagsToCaptureDefs.Add(Tags.Attribute_Secondary_CriticalHitChance,CriticalHitChanceDef);
    	TagsToCaptureDefs.Add(Tags.Attribute_Secondary_CriticalHitResistance,CriticalHitResistanceDef);
    	TagsToCaptureDefs.Add(Tags.Attribute_Secondary_CriticalHitChance,CriticalHitDamageDef);
    	TagsToCaptureDefs.Add(Tags.Attribute_Resistance_Fire,FireResistanceDef);
    	TagsToCaptureDefs.Add(Tags.Attribute_Resistance_Arcane,ArcaneResistanceDef);
    	TagsToCaptureDefs.Add(Tags.Attribute_Resistance_Lightning,LightningResistanceDef);
    	TagsToCaptureDefs.Add(Tags.Attribute_Resistance_Physical,PhysicalResistanceDef);
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
	RelevantAttributesToCapture.Add(DamageStatics().FireResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().LightningResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().PhysicalResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArcaneResistanceDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC=ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC=ExecutionParams.GetTargetAbilitySystemComponent();

    AActor* SourceAvatar=SourceASC->GetAvatarActor();
    AActor* TargetAvatar=TargetASC->GetAvatarActor();

	int32 SourcePlayerLevel=1;
	if (SourceAvatar->Implements<UCombatInterface>())
	{
		SourcePlayerLevel=ICombatInterface::Execute_GetLevel(SourceAvatar);
	}

	int32 TargetPlayerLevel=1;
	if (TargetAvatar->Implements<UCombatInterface>())
	{
		TargetPlayerLevel=ICombatInterface::Execute_GetLevel(TargetAvatar);
	}

	const FGameplayEffectSpec& EffectSpec=ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags=EffectSpec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags=EffectSpec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluatedParams;
	EvaluatedParams.SourceTags=SourceTags;
	EvaluatedParams.TargetTags=TargetTags;

	
    // Get Damage Set by Caller Magnitude

	float Damage=0.f;
	for (auto Pair : FBaseGameplayTags::Get().DamageTypesToResistances)
	{
		const FGameplayTag DamageTypeTag=Pair.Key;
		const FGameplayTag ResistanceTypeTag=Pair.Value;

		const FGameplayEffectAttributeCaptureDefinition CaptureDef= AuraDamageStatics().TagsToCaptureDefs[ResistanceTypeTag];
		
		float DamageTypeValue=EffectSpec.GetSetByCallerMagnitude(DamageTypeTag);
		float ResistanceValue=0.f;

		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef,EvaluatedParams,ResistanceValue);
		ResistanceValue=FMath::Clamp(ResistanceValue,0.f,100.f);
		DamageTypeValue=DamageTypeValue*(100.f-ResistanceValue)/100.f;
		Damage+=DamageTypeValue;
	}
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

	FGameplayEffectContextHandle EffectContextHandle=EffectSpec.GetContext();
	UAuraAbilitySystemLibrary::SetIsBlockedHit(EffectContextHandle,bBlocked);

	UCharacterClassInfo* ClassInfo=UAuraAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);
	FRealCurve* ArmorPenetrationCurve=ClassInfo->DamageCalculationCoefficient->FindCurve(FName("ArmorPenetration"),FString());
	float ArmorPenetrationCoefficient=ArmorPenetrationCurve->Eval(SourcePlayerLevel);
	
	// ArmorPenetration ignores a percentage of the Target's Armor.	
	const float EffectiveArmor = TargetArmor * ( 100 - SourceArmorPenetration * ArmorPenetrationCoefficient ) / 100.f;

	FRealCurve* EffectiveArmorCurve=ClassInfo->DamageCalculationCoefficient->FindCurve(FName("EffectiveArmor"),FString());
	float EffectiveArmorCoefficient=EffectiveArmorCurve->Eval(TargetPlayerLevel);
	
	// Armor ignores a percentage of incoming Damage.
	Damage =Damage* ( 100 - EffectiveArmor * EffectiveArmorCoefficient ) / 100.f;

	FRealCurve* CriticalHitResistanceCurve=ClassInfo->DamageCalculationCoefficient->FindCurve(FName("CriticalHitResistance"),FString());
	float CriticalHitResistanceCoefficient = CriticalHitResistanceCurve->Eval(TargetPlayerLevel);

	const float EffectiveCriticalHitChance = SourceCriticalHitChance - TargetCriticalHitResistance * CriticalHitResistanceCoefficient;
	const bool bCriticalHit=FMath::RandRange(0.f,100.f)<EffectiveCriticalHitChance;
	UAuraAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle,bCriticalHit);
	Damage=bCriticalHit ? Damage*2.f+SourceCriticalHitDamage : Damage;
	
	const FGameplayModifierEvaluatedData EvaluatedData(UBaseAttributeSet::GetIncomingDamageAttribute(),EGameplayModOp::Additive,Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
