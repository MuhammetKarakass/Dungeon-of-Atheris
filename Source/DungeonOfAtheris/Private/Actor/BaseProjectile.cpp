// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/BaseProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


ABaseProjectile::ABaseProjectile()
{

	PrimaryActorTick.bCanEverTick = false;
    bReplicates = true;
	
	Sphere=CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(Sphere);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic,ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);

	ProjectileMovement=CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed=550;
	ProjectileMovement->MaxSpeed=550;
	ProjectileMovement->ProjectileGravityScale=0.f;

}


void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	Sphere->OnComponentBeginOverlap.AddDynamic(this,&ABaseProjectile::OnSphereOverlap);
	
}

void ABaseProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}


