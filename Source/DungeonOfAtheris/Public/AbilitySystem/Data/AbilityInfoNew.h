// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AbilityInfoNew.generated.h"
class UGameplayAbility;

USTRUCT(BlueprintType)
struct FAuraAbilityInfoNew
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FGameplayTag AbilityTag=FGameplayTag();

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag InputTag=FGameplayTag();

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FGameplayTag CooldownTag=FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AbilityType = FGameplayTag();

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TObjectPtr<UTexture2D> AbilityIcon=nullptr;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TObjectPtr<UMaterialInterface> BackgroundMaterial=nullptr;

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag StatusTag=FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 LevelRequirement = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> Ability;
};


UCLASS()
class DUNGEONOFATHERIS_API UAbilityInfoNew : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category="AbilityInformation")
	TArray<FAuraAbilityInfoNew> AuraAbilityInformation;

	FAuraAbilityInfoNew FındAbilityInformationForTag(FGameplayTag AbilityTag,const bool bLogNotFound=false) const;
};
