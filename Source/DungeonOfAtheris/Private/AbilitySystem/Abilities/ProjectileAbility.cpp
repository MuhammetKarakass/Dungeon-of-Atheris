// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/ProjectileAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "BaseGameplayTags.h"
#include "Actor/BaseProjectile.h"
#include "Interaction/CombatInterface.h"

void UProjectileAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                         const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                         const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	
}

void UProjectileAbility::SpawnProjectile(const FVector& ProjectileTargetLocation)
{
	const bool bIsServer=GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	ICombatInterface *CombatInterface=Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInterface)
	{
		const FVector SocketLocation=CombatInterface->GetSocketLocation();
		FRotator Rotation=(ProjectileTargetLocation-SocketLocation).Rotation();
		Rotation.Pitch=0.f;
		FTransform SpawnTransform;
		SpawnTransform.SetRotation(Rotation.Quaternion());
		SpawnTransform.SetLocation(SocketLocation);

		ABaseProjectile* Projectile=GetWorld()->SpawnActorDeferred<ABaseProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetAvatarActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		
		const UAbilitySystemComponent* ASC=UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
		const FGameplayEffectSpecHandle SpecHandle=ASC->MakeOutgoingSpec(DamageEffectClass,GetAbilityLevel(),ASC->MakeEffectContext());
		
		const float ScalableDamage=Damage.GetValueAtLevel(15);
		FBaseGameplayTags GameplayTags=FBaseGameplayTags::Get();
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle,GameplayTags.Damage,ScalableDamage);
		Projectile->DamageSpecHandle=SpecHandle;
		Projectile->FinishSpawning(SpawnTransform);
	}
}
