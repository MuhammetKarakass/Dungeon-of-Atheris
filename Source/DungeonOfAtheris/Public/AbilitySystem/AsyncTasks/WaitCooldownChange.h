// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "WaitCooldownChange.generated.h"

struct FGameplayTag;
class UAbilitySystemComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCoolDownChangeSignature, float, TimeRemaining);

/**
 * 
 */
UCLASS(BlueprintType,meta=(ExposedAsyncProxy="AsyncTask"))
class DUNGEONOFATHERIS_API UWaitCooldownChange : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FCoolDownChangeSignature CooldownStart;

	UPROPERTY(BlueprintAssignable)
	FCoolDownChangeSignature CooldownEnd;

	UFUNCTION(BlueprintCallable,meta=(BlueprintInternalUseOnly="true"))
	static UWaitCooldownChange* WaitForCooldownChange(UAbilitySystemComponent* InAbilitySystemComponent,FGameplayTag InCooldownTag);

	UFUNCTION(BlueprintCallable)
	void EndTask();

protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	FGameplayTag CooldownTag;

	void CoolDownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount);
	void OnActiveEffectAdded(UAbilitySystemComponent* TargetASC,const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveGameplayEffect);
};
