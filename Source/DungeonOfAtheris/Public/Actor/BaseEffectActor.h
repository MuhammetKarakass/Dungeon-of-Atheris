// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameFramework/Actor.h"
#include "BaseEffectActor.generated.h"

class UGameplayEffect;
class USphereComponent;

UENUM(BlueprintType)
enum class EEffectApplicationPolicy: uint8
{
	ApplyOnOverlap,
	ApplyOnEndOverlap,
	DoNotApply
};

UENUM(BlueprintType)
enum class EEffectRemovalPolicy: uint8
{
	RemoveOnEndOverlap,
	DoNotRemove
};

USTRUCT(BlueprintType)
struct FGameplayEffectStruct
{
	GENERATED_BODY()
	FGameplayEffectStruct() {}

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Applied Effect")
    TSubclassOf<UGameplayEffect> GameplayEffectClass;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Applied Effect")
    EEffectApplicationPolicy EffectApplicationPolicy=EEffectApplicationPolicy::DoNotApply;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Applied Effect")
    EEffectRemovalPolicy EffectRemovalPolicy=EEffectRemovalPolicy::RemoveOnEndOverlap;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Applied Effect")
	float EffectLevel=1.f;

};

UCLASS()
class DUNGEONOFATHERIS_API ABaseEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseEffectActor();

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Applied Effect")
    TArray<FGameplayEffectStruct> GameplayEffectStructs;
	
	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* TargetActor);
	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor);
	
	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* ActorTarget,const FGameplayEffectStruct& GameplayEffectStruct);

	//2 veriyi tutuyoruz
	TMap<FActiveGameplayEffectHandle,UAbilitySystemComponent*> ActiveGameplayEffects;


	
	// UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Applied Effect")
	// TArray<TSubclassOf<UGameplayEffect>> InstantGameplayEffect;
	//
	// UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Applied Effect")
	//    EEffectApplicationPolicy InstantEffectApplicationPolicy=EEffectApplicationPolicy::DoNotApply;
	//
	// UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Applied Effect")
	// TArray<TSubclassOf<UGameplayEffect>> DurationGameplayEffect;
	//
	// UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Applied Effect")
	// EEffectApplicationPolicy DurationEffectApplicationPolicy=EEffectApplicationPolicy::DoNotApply;
	//
	// UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Applied Effect")
	// TArray<TSubclassOf<UGameplayEffect>> InfiniteGameplayEffect;
	//
	// UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Applied Effect")
	// EEffectApplicationPolicy InfiniteEffectApplicationPolicy=EEffectApplicationPolicy::DoNotApply;    
	//
	// UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Applied Effect")
	// EEffectRemovalPolicy InfiniteEffectRemovalPolicy=EEffectRemovalPolicy::RemoveOnEndOverlap;
};
