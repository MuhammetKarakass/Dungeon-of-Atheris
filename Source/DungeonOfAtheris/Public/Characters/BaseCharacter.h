// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

UCLASS(Abstract) //Abstract means can't instanciate in the game
class DUNGEONOFATHERIS_API ABaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	ABaseCharacter();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override; //AbilitySystemInterface function for get ability system component
	UAttributeSet* GetAttributeSet() const {return AttributeSet;}

protected:

	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo();

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
};
