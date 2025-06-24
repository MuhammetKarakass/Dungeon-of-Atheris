// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/ProjectileAbility.h"
#include "AuraFireBoltAbility.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONOFATHERIS_API UAuraFireBoltAbility : public UProjectileAbility
{
	GENERATED_BODY()

public:

	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;
	
};
