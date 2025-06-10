// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/AbilityInfoNew.h"

FAuraAbilityInfoNew UAbilityInfoNew::FındAbilityInformationForTag(FGameplayTag AbilityTag, const bool bLogNotFound) const
{
	for (FAuraAbilityInfoNew AbilityInfo : AuraAbilityInformation)
	{
		if (AbilityTag==AbilityInfo.AbilityTag)
		{
			return AbilityInfo;
		}
	}
	if (bLogNotFound)
	{
		
	}
	return FAuraAbilityInfoNew();
}