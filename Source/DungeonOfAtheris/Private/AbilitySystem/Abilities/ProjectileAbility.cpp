// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/ProjectileAbility.h"

#include "Actor/BaseProjectile.h"
#include "Interaction/CombatInterface.h"

void UProjectileAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                         const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                         const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	const bool bIsServer=HasAuthority(&ActivationInfo);
	if (!bIsServer) return;

	ICombatInterface *CombatInterface=Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		const FVector SocketLocation=CombatInterface->GetSocketLocation();
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);

		ABaseProjectile* Projectile=GetWorld()->SpawnActorDeferred<ABaseProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetAvatarActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		Projectile->FinishSpawning(SpawnTransform);
	}
}
