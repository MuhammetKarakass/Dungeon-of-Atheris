// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/BaseEffectActor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

#include "AbilitySystemInterface.h"

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
		if (EffectStruct.EffectApplicationPolicy==EEffectApplicationPolicy::ApplyOnOverlap)
		{
			ApplyEffectToTarget(TargetActor, EffectStruct);
		}
	}
}

void ABaseEffectActor::OnEndOverlap(AActor* TargetActor)
{
	for (const FGameplayEffectStruct& EffectStruct:GameplayEffectStructs)
	{
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
	if (UAbilitySystemComponent* TargetASC= UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(ActorTarget))
	{
		check(GameplayEffectStruct.GameplayEffectClass);
		// Efektin nereden geldiği, nasıl tetiklendiği gibi bilgileri içerir.
		FGameplayEffectContextHandle EffectContextHandle= TargetASC->MakeEffectContext();
		//Efektin kaynağını belirtmek.
		EffectContextHandle.AddSourceObject(this);
		//Efektin işlevselliğini tanımlar (örneğin, ne kadar hasar vereceği veya hangi özellikleri değiştireceği gibi).
		const FGameplayEffectSpecHandle EffectSpecHandle=TargetASC->MakeOutgoingSpec(GameplayEffectStruct.GameplayEffectClass,EffectLevel,EffectContextHandle);
		//Tanımlanan efekt hedefe uygulanır.
		const FActiveGameplayEffectHandle ActiveEffectHandle=TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

		const bool bIsInfinite=EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy==EGameplayEffectDurationType::Infinite;

		if (bIsInfinite && GameplayEffectStruct.EffectRemovalPolicy==EEffectRemovalPolicy::RemoveOnEndOverlap)
		{
			ActiveGameplayEffects.Add(ActiveEffectHandle,TargetASC);
		}
	}
}


