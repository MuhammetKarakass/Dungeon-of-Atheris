// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/BaseProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemLibrary.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "DungeonOfAtheris/DungeonOfAtheris.h"

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
	Sphere->SetCollisionObjectType(ECC_Projectile);

	ProjectileMovement=CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed=550;
	ProjectileMovement->MaxSpeed=550;
	ProjectileMovement->ProjectileGravityScale=0.f;

}


void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	Sphere->OnComponentBeginOverlap.AddDynamic(this,&ABaseProjectile::OnSphereOverlap);
	LoopingSound = UGameplayStatics::SpawnSoundAttached(FloatingSound,GetRootComponent());
	SetLifeSpan(LifeSpan);
	
}

void ABaseProjectile::Destroyed()
{
	if (!HasAuthority()&&!bHit)
	{
		UGameplayStatics::PlaySoundAtLocation(this,ImpactSound,GetActorLocation());
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this,ImpactEffect,GetActorLocation());
		bHit=true;
	}
	Super::Destroyed();
}

void ABaseProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// AActor* Acctor=DamageSpecHandle.Data.Get()->GetContext().GetEffectCauser();
	// if (Acctor==OtherActor && DamageSpecHandle.Data.IsValid()) return;

	if (!DamageSpecHandle.Data.IsValid() || DamageSpecHandle.Data.Get()->GetContext().GetEffectCauser() == OtherActor)
	{
		return;
	}

	if (!UAuraAbilitySystemLibrary::IsNotFriends(DamageSpecHandle.Data.Get()->GetContext().GetEffectCauser(), OtherActor))
	{
		return;
	}
	
	if (!bHit)
	{
		UGameplayStatics::PlaySoundAtLocation(this,ImpactSound,GetActorLocation());
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this,ImpactEffect,GetActorLocation());
		if (LoopingSound)LoopingSound->Stop();
		bHit=true;
	}

	if (HasAuthority())
	{
		Destroy();
		if (UAbilitySystemComponent* TargetASC=UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			TargetASC->ApplyGameplayEffectSpecToSelf(*DamageSpecHandle.Data.Get());
		}
	}
	else
	{
		bHit=true;
	}
}
