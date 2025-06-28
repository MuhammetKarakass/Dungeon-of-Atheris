// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAbilityTypes.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "BaseProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class UNiagaraSystem;

UCLASS()
class DUNGEONOFATHERIS_API ABaseProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(visibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;
	ABaseProjectile();

	UPROPERTY(BlueprintReadWrite,meta=(ExposeOnSpawn = true))
	FDamageEffectParams DamageEffectParams;

	UPROPERTY()
	TObjectPtr<USceneComponent> HomingTargetSceneComponent;
	
	bool IsValidOverlap(AActor* OtherActor);
	bool bHit=false;
protected:

	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	void OnHit();
	virtual void Destroyed() override;
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	UPROPERTY(VisibleAnywhere);
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> ImpactEffect;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> ImpactSound;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> FloatingSound;

	UPROPERTY(EditDefaultsOnly)
	float LifeSpan=15.f;

	UPROPERTY()
	TObjectPtr<UAudioComponent> LoopingSound;

};