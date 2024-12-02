// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Interaction/EnemyInterface.h"
#include "EnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONOFATHERIS_API AEnemyCharacter : public ABaseCharacter,public IEnemyInterface
{
	GENERATED_BODY()

public:

	AEnemyCharacter();

	// begin Enemy Interface
	virtual void HiglightActor() override;
	virtual void UnHiglightActor() override;
	// end Enemy Interface

protected:
	virtual void BeginPlay() override;
};
