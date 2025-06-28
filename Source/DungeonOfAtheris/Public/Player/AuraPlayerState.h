// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "AuraPlayerState.generated.h"

class ULevelUpInfo;
class UAbilitySystemComponent;
class UAttributeSet;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChanged,int32)
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnLevelChanged, int32 , bool)

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
	FORCEINLINE int32 GetXP() const {return XP;}
	FORCEINLINE int32 GetAttributePoints () const {return AttributePoints;}
	FORCEINLINE int32 GetSpellPoints() const {return SpellPoints;}

	void AddToXP(int32 InXP);
	void AddToLevel(int32 InLevel);
	void AddToAttributePoints(int32 InAttributePoints);
	void AddToSpellPoints(int32 InSpellPoints);

	void SetXP(int32 InXP);
	void SetLevel(int32 InLevel);
	void SetAttributePoints(int32 InAttributePoints);
	void SetSpellPoints(int32 InSpellPoints);
	

	FOnPlayerStatChanged OnXPChangedDelegate;
	FOnLevelChanged OnLevelCanChangeDelegate;
	FOnPlayerStatChanged OnAttributePointsCanChangeDelegate;
	FOnPlayerStatChanged OnSpellPointsCanChangeDelegate;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TObjectPtr<ULevelUpInfo> LevelUpInfo;
	
protected:
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

private:
	UPROPERTY(VisibleAnywhere,ReplicatedUsing=OnRep_PlayerLevel)
	int32 PlayerLevel=1;

	UPROPERTY(VisibleAnywhere,ReplicatedUsing=OnRep_XP)
	int32 XP=0;

	UPROPERTY(VisibleAnywhere,ReplicatedUsing=OnRep_AttributePoints)
	int32 AttributePoints=0;

	UPROPERTY(VisibleAnywhere,ReplicatedUsing=OnRep_SpellPoints)
	int32 SpellPoints=0;

	UFUNCTION()
	void OnRep_PlayerLevel(int32 OldLevel);

	UFUNCTION()
	void OnRep_XP(int32 OldXP);

	UFUNCTION()
	void OnRep_AttributePoints(int32 OldAttributePoints);

	UFUNCTION()
	void OnRep_SpellPoints(int32 OldSpellPoints);
};
