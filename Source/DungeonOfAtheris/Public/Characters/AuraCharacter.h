// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "Interaction/PlayerInterface.h"
#include "AuraCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UNiagaraComponent;

UCLASS()
class DUNGEONOFATHERIS_API AAuraCharacter : public ABaseCharacter,public IPlayerInterface
{
	GENERATED_BODY()
	
public:

	AAuraCharacter();
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Camera")
	TObjectPtr<USpringArmComponent> SpringArmComp;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Camera")
	TObjectPtr<UCameraComponent> CameraComp;

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	virtual void InitAbilityActorInfo() override;

	virtual int32 GetLevel_Implementation() override;
	//Player Interface
	virtual void AddToXP_Implementation(int32 InXP) override;
	virtual void LevelUp_Implementation() override;
	virtual int32 GetXP_Implementation() override;
	virtual int32 FindLevelForXP_Implementation(int32 InXP) override;
	virtual int32 GetSpellPointsReward_Implementation(int32 InPlayerLevel) override;
	virtual int32 GetAttributePointsReward_Implementation(int32 InPlayerLevel) override;
	virtual void AddToAttributePoints_Implementation(int32 InAttributePoints) override;
	virtual void AddToSpellPoints_Implementation(int32 InSpellPoints) override;
	virtual void AddToPlayerLevel_Implementation(int32 InPlayerLevel) override;
	virtual int32 GetAttributePoints_Implementation() override;
	virtual int32 GetSpellPoints_Implementation() override;
	virtual void SaveProgress_Implementation(const FName& CheckpointTag) override;
	//end Player Interface

	virtual void OnRep_Stunned() override;
	virtual void OnRep_Burned() override;

	void LoadProgress();
private:

	UFUNCTION(NetMulticast,Reliable)
	void MultiCastLevelUpParticles() const;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraComponent> LevelUpNiagaraComponent;
};
