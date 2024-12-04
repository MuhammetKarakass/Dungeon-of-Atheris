// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/BaseEffectActor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "Components/SphereComponent.h"

// Sets default values
ABaseEffectActor::ABaseEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh=CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);

	Sphere=CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(GetRootComponent());
}

void ABaseEffectActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//TODO: Change to gameplay effect
	if (IAbilitySystemInterface* ABCInterface=Cast<IAbilitySystemInterface>(OtherActor))
	{
		const UBaseAttributeSet *AttributeSet = Cast<UBaseAttributeSet>(ABCInterface->GetAbilitySystemComponent()->GetAttributeSet(UBaseAttributeSet::StaticClass()));
		UBaseAttributeSet* MutableAttributeSet=const_cast<UBaseAttributeSet*>(AttributeSet);
		MutableAttributeSet->SetHealth()
	}
}

void ABaseEffectActor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void ABaseEffectActor::BeginPlay()
{
	Super::BeginPlay();
	Sphere->OnComponentBeginOverlap.AddDynamic(this,&ABaseEffectActor::OnOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this,&ABaseEffectActor::EndOverlap);
	
}


