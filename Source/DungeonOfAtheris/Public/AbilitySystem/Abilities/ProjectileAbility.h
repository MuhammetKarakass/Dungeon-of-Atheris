// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/DamageGameplayAbility.h"
#include "ProjectileAbility.generated.h"

class ABaseProjectile;
/**
 * 
 */
UCLASS()
class DUNGEONOFATHERIS_API UProjectileAbility : public UDamageGameplayAbility
{
	GENERATED_BODY()


protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<ABaseProjectile> ProjectileClass;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable)
	void SpawnProjectile(const FVector& ProjectileTargetLocation,const FGameplayTag& Tag);

	UPROPERTY(EditDefaultsOnly)
	int32 NumProjectiles = 5;
};
