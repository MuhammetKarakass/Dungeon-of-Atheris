// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "BaseCharacter.generated.h"

class UGameplayAbility;
class UAbilitySystemComponent;
class UAttributeSet;

UCLASS(Abstract) //Abstract means can't instance in the game
class DUNGEONOFATHERIS_API ABaseCharacter : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:

	ABaseCharacter();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override; //AbilitySystemInterface function for get ability system component
	UAttributeSet* GetAttributeSet() const {return AttributeSet;}

protected:

	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo();
	virtual FVector GetSocketLocation();
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(EditAnywhere, Category = "Combat")
	FName WeaponTipSocketName;
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(BlueprintReadOnly,EditAnywhere,category="Attributes")
	TSubclassOf<class UGameplayEffect> DefaultPrimaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<class UGameplayEffect> DefaultSecondaryAttributes;

	UPROPERTY(BlueprintReadOnly,EditAnywhere, Category = "Attributes")
	TSubclassOf<class UGameplayEffect> DefaultVitalAttributes;

	void ApplyEffectToSelf(TSubclassOf<class UGameplayEffect> Effect, float level) const;
	void InitDefaultAttributes() const;

	void AddCharacterAbilities();

private:
	UPROPERTY(EditAnywhere, Category="Abilites")
	TArray<TSubclassOf<UGameplayAbility>> Abilities;
};
