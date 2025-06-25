// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "AuraPlayerController.generated.h"

class UNiagaraSystem;
class UDamageTextComponent;
class USplineComponent;
class UBaseAbilitySystemComponent;
class UInputMappingContext;
class UInputAction;
class IEnemyInterface;
class UAuraInputConfig;
struct FInputActionValue;

UCLASS()
class DUNGEONOFATHERIS_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AAuraPlayerController();

	virtual void PlayerTick(float DeltaTime) override;

	UFUNCTION(Client,Reliable)
	void ShowDamageNumber(float DamageAmmount,ACharacter* TargetCharacter,bool bIsBlockedHit, bool bIsCriticalHit);
	
protected:

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	
private:

	void Move(const FInputActionValue& InputActionValue);
	void CursorTrace();
	void AutoRun();

	void AbilityInputTagPressed(FGameplayTag Tag);
	void AbilityInputTagReleased(FGameplayTag Tag);
	void AbilityInputTagHeld(FGameplayTag Tag);

	bool bShiftKeyDown=false;
	void ShiftKeyPressed(){bShiftKeyDown=true;}
	void ShiftKeyReleased(){bShiftKeyDown=false;}
	UBaseAbilitySystemComponent* GetASC();

	TScriptInterface<IEnemyInterface> CurrentActor;
	TScriptInterface<IEnemyInterface> LastActor;
	
	UPROPERTY(EditAnywhere,Category = Input)
	TObjectPtr<UInputMappingContext> AuraContext;

	UPROPERTY(EditAnywhere,Category = Input)
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere,Category = Input)
	TObjectPtr<UInputAction>ShiftAction;

	UPROPERTY(EditAnywhere,Category = Input)
	TObjectPtr<UAuraInputConfig>InputConfig;

	UPROPERTY(EditDefaultsOnly)
    TObjectPtr<UNiagaraSystem> ClickNiagaraSystem;
	
	UPROPERTY()
	TObjectPtr <UBaseAbilitySystemComponent> AbilitySystemComponent;

	FVector CachedDestination=FVector::ZeroVector;
	float FollowTime=0.f;
	UPROPERTY(EditAnywhere)
	float ShorPressedTime=0.5f;
	bool bAutoRunning=false;
	bool bTargeting=false;

	UPROPERTY(EditAnywhere)
	float AutoRunAcceptanceRadius=50.f;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;
};
