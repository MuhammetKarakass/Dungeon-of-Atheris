// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/AbilityInfo.h"

FAuraAbilityInfo UAbilityInfo::FındAbilityInformationForTag(FGameplayTag AbilityTag, const bool bLogNotFound) const
{
	for (FAuraAbilityInfo AbilityInfo : AuraAbilityInformation)
	{
		if (AbilityTag==AbilityInfo.AbilityTag)
		{
			return AbilityInfo;
		}
	}
	if (bLogNotFound)
	{
		
	}
	return FAuraAbilityInfo();
}
