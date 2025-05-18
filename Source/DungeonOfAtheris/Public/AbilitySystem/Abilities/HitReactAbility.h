// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/BaseGameplayAbility.h"
#include "HitReactAbility.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONOFATHERIS_API UHitReactAbility : public UBaseGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Combat")
	TSubclassOf<UGameplayEffect> HitReact;

	UFUNCTION(BlueprintCallable)
	FActiveGameplayEffectHandle GetHitReactEffect();
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
};
