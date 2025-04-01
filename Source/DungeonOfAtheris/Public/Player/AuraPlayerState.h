// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "AuraPlayerState.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

UCLASS()
class DUNGEONOFATHERIS_API AAuraPlayerState : public APlayerState,public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	AAuraPlayerState();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const {return AttributeSet;}

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	FORCEINLINE int32 GetPlayerLevel() const {return PlayerLevel;}
	
protected:
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

private:
	UPROPERTY(VisibleAnywhere,ReplicatedUsing=OnRep_PlayerLevel)
	int32 PlayerLevel=1;

	UFUNCTION()
	void OnRep_PlayerLevel(int32 OldLevel);
};
