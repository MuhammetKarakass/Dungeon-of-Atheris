// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ModMagCalc/MMC_MaxHealth.h"

#include "AbilitySystem/BaseAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	VigorDef.AttributeToCapture=UBaseAttributeSet::GetVigorAttribute();
	VigorDef.AttributeSource=EGameplayEffectAttributeCaptureSource::Target;
	VigorDef.bSnapshot=false;
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* TargetContainer=Spec.CapturedTargetTags.GetAggregatedTags();
	const FGameplayTagContainer* SourceContainer=Spec.CapturedSourceTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.TargetTags=TargetContainer;
	EvaluateParameters.SourceTags=SourceContainer;

	float Vigor=0;
	GetCapturedAttributeMagnitude(VigorDef,Spec,EvaluateParameters,Vigor);

	int32 PlayerLevel=1;
	
	if (Spec.GetContext().GetSourceObject()->Implements<UCombatInterface>())
	{
		PlayerLevel=ICombatInterface::Execute_GetLevel(Spec.GetContext().GetSourceObject());
	};

	return 80.f + Vigor * 2.5 + PlayerLevel * 10.f;
}
