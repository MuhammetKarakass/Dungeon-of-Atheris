// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "AuraCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class DUNGEONOFATHERIS_API AAuraCharacter : public ABaseCharacter
{
	GENERATED_BODY()

	AAuraCharacter();

public:
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Camera")
	TObjectPtr<USpringArmComponent> SpringArmComp;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Camera")
	TObjectPtr<UCameraComponent> CameraComp;



	
	
};
