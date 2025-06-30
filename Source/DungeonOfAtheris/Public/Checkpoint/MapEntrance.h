// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Checkpoint/Checkpoint.h"
#include "MapEntrance.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONOFATHERIS_API AMapEntrance : public ACheckpoint
{
	GENERATED_BODY()

	AMapEntrance(const FObjectInitializer& ObjectInitializer);

	
	virtual void HiglightActor_Implementation() override;
	
	virtual void LoadActor_Implementation() override;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UWorld> DestinationMap;

	UPROPERTY(EditAnywhere)
	FName DestinationPlayerStartTag;
	
protected:
	
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
								 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
								 const FHitResult& SweepResult) override;
	
};
