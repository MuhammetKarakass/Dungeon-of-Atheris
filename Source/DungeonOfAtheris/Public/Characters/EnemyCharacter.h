// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Interaction/EnemyInterface.h"
#include "Interaction/HighlightInterface.h"
#include  "UI/WidgetController/OverlayWidgetController.h"
#include "EnemyCharacter.generated.h"

class ABaseAIController;
class UBehaviorTree;
class UWidgetComponent;
/**
 * 
 */
UCLASS()
class DUNGEONOFATHERIS_API AEnemyCharacter : public ABaseCharacter,public IEnemyInterface,public IHighlightInterface
{
	GENERATED_BODY()

public:

	AEnemyCharacter();

	// begin Enemy Interface
	virtual void HiglightActor_Implementation() override;
	virtual void UnHiglightActor_Implementation() override;
	virtual void Die(const FVector& DeathImpulse) override;
	virtual void SetMoveToLocation_Implementation(FVector& OutDestination) override;
	// end Enemy Interface

	virtual int32 GetLevel_Implementation() override;

	void HitReactTagChanged(const FGameplayTag CallbackTag,int32 NewCount);

	virtual void PossessedBy(AController* NewController) override;

	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;
	virtual AActor* GetCombatTarget_Implementation() override;
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangeSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangeSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintReadOnly,Category="Combat")
	bool bHitReacting=false;
	

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Combat")
	float LifeSpan=5.f;

	void SetLevel(int32 InLevel) { Level = InLevel; }
	
protected:
	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo() override;

	virtual void InitDefaultAttributes() const override;

	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount) override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Character Class Defaults")
	int32 Level=1;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

	UPROPERTY(EditDefaultsOnly,Category="AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<AActor> CombatTarget;

	UPROPERTY()
	TObjectPtr<ABaseAIController> AIController;

	UFUNCTION(BlueprintImplementableEvent)
	void SpawnLoot();
};

inline void AEnemyCharacter::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
	CombatTarget = InCombatTarget;
}

inline AActor* AEnemyCharacter::GetCombatTarget_Implementation()
{
	return CombatTarget;
}
