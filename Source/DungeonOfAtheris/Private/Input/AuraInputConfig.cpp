// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/AuraInputConfig.h"

const UInputAction* UAuraInputConfig::FindAbilityInputActionForTag(const FGameplayTag& GameplayTag,
	bool bLogNotFound) const
{
	for (const FAuraInputAction& Action: AbilityInputActions )
	{
		if (Action.InputAction  && Action.InputTag == GameplayTag)
		{
			return Action.InputAction;
		}
	}
	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find AbilityAction for InputTag[%s], on InputConfig[%s]"),*GameplayTag.ToString(),*GetNameSafe(this));
	}
	return nullptr;
}
