// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/BaseEffectActor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

// Sets default values
ABaseEffectActor::ABaseEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;
	
	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent")));
}

void ABaseEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseEffectActor::OnOverlap(AActor* TargetActor)
{
	for (const FGameplayEffectStruct& EffectStruct:GameplayEffectStructs)
	{
		if (TargetActor->ActorHasTag(FName("Enemy")) && !EffectStruct.bApplyEffectsToEnemies) return;
		if (EffectStruct.EffectApplicationPolicy==EEffectApplicationPolicy::ApplyOnOverlap)
		{
			ApplyEffectToTarget(TargetActor, EffectStruct);
		}
	}
	// if (IAbilitySystemInterface* ACSInterface=Cast<IAbilitySystemInterface>(TargetActor))
	// {
	// 	const UBaseAttributeSet* Attributeset=Cast<UBaseAttributeSet>(ACSInterface->GetAbilitySystemComponent()->GetAttributeSet(UAttributeSet::StaticClass()));
	// 	UBaseAttributeSet* Mutable=const_cast<UBaseAttributeSet*>(Attributeset);
	// 	Mutable->SetHealth(Mutable->GetHealth()+100.0f);
	// }
}

void ABaseEffectActor::OnEndOverlap(AActor* TargetActor)
{
	for (const FGameplayEffectStruct& EffectStruct:GameplayEffectStructs)
	{
		if (TargetActor->ActorHasTag(FName("Enemy")) && !EffectStruct.bApplyEffectsToEnemies) return;
		if (EffectStruct.EffectApplicationPolicy==EEffectApplicationPolicy::ApplyOnEndOverlap)
		{
			ApplyEffectToTarget(TargetActor, EffectStruct);
		}

		if (EffectStruct.EffectRemovalPolicy==EEffectRemovalPolicy::RemoveOnEndOverlap)
		{
			UAbilitySystemComponent* TargetASC=UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
			if (!IsValid(TargetASC)) return;
			TArray<FActiveGameplayEffectHandle> EffectsToRemove;
			for (TTuple<FActiveGameplayEffectHandle, UAbilitySystemComponent*> HandlePairs:ActiveGameplayEffects)
			{
				if (HandlePairs.Value==TargetASC)
				{
					TargetASC->RemoveActiveGameplayEffect(HandlePairs.Key,1);
					EffectsToRemove.Add(HandlePairs.Key);
				}
			}
			for (FActiveGameplayEffectHandle EffectRemove:EffectsToRemove)
			{
				ActiveGameplayEffects.Remove(EffectRemove);
			}
		}
	}
}

void ABaseEffectActor::ApplyEffectToTarget(AActor* ActorTarget,const FGameplayEffectStruct& GameplayEffectStruct)
{
	if (ActorTarget->ActorHasTag(FName("Enemy")) && !GameplayEffectStruct.bApplyEffectsToEnemies) return;
	if (UAbilitySystemComponent* TargetASC= UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(ActorTarget))
	{
		check(GameplayEffectStruct.GameplayEffectClass);
		// Efektin nereden geldiği, nasıl tetiklendiği gibi bilgileri içerir.
		FGameplayEffectContextHandle EffectContextHandle= TargetASC->MakeEffectContext();
		//Efektin kaynağını belirtmek.
		EffectContextHandle.AddSourceObject(this);
		//Efektin işlevselliğini tanımlar (örneğin, ne kadar hasar vereceği veya hangi özellikleri değiştireceği gibi).
		const FGameplayEffectSpecHandle EffectSpecHandle=TargetASC->MakeOutgoingSpec(GameplayEffectStruct.GameplayEffectClass,GameplayEffectStruct.EffectLevel,EffectContextHandle);
		//Tanımlanan efekt hedefe uygulanır.
		const FActiveGameplayEffectHandle ActiveEffectHandle=TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

		const bool bIsInfinite=EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy==EGameplayEffectDurationType::Infinite;

		if (bIsInfinite && GameplayEffectStruct.EffectRemovalPolicy==EEffectRemovalPolicy::RemoveOnEndOverlap)
		{
			ActiveGameplayEffects.Add(ActiveEffectHandle,TargetASC);
		}

		if (!bIsInfinite && GameplayEffectStruct.bDestroyedOnEffectApplication)
		{
			Destroy();
		}
	}
}


