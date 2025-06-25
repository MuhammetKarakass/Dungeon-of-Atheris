

#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "BaseGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Input/AuraInputComponent.h"
#include "Input/AuraInputConfig.h"
#include "Interaction/EnemyInterface.h"
#include "AbilitySystem/BaseAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Character.h"
#include "UI/UserWidget/DamageTextComponent.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
	Spline=CreateDefaultSubobject<USplineComponent>("Spline");
	
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
	AutoRun();
}

void AAuraPlayerController::ShowDamageNumber_Implementation(float DamageAmmount, ACharacter* TargetCharacter,bool bIsBlockedHit,bool bIsCriticalHit)
{
	if (IsValid(TargetCharacter)&& DamageTextComponentClass&&IsLocalController())
	{
		UDamageTextComponent* DamageTextComponent = NewObject<UDamageTextComponent>(TargetCharacter,DamageTextComponentClass);
		DamageTextComponent->RegisterComponent();//createdefaultsubobject gibi ama dinamik olarak oluşturduk ondan
		DamageTextComponent->AttachToComponent(TargetCharacter->GetRootComponent(),FAttachmentTransformRules::KeepRelativeTransform);
		DamageTextComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageTextComponent->SetDamageText(DamageAmmount,bIsBlockedHit,bIsCriticalHit);
	}
}

void AAuraPlayerController::AutoRun()
{
	if (!bAutoRunning) return;

	if (APawn* ControlledPawn = GetPawn())
	{
		const FVector LocationOnSpline=Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector Direction=Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(Direction);

		const float DistanceToDestination=(LocationOnSpline-CachedDestination).Length();
		if (DistanceToDestination<=AutoRunAcceptanceRadius)
		{
			bAutoRunning=false;
		}
	}
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag Tag)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FBaseGameplayTags::Get().Player_Block_InputPressed))
	{
		return;
	}
	
	if (Tag.MatchesTagExact(FBaseGameplayTags::Get().InputTag_LMB))
	{
		bTargeting=CurrentActor ? true : false;
		bAutoRunning=false;
	}
	if (GetASC()) GetASC()->AbilityInputTagPressed(Tag);
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag Tag)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FBaseGameplayTags::Get().Player_Block_InputReleased))
	{
		return;
	}
	
	if (!Tag.MatchesTagExact(FBaseGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagReleased(Tag);
		}
		return;
	}

	if (GetASC())
	{
		GetASC()->AbilityInputTagReleased(Tag);
	}
	
	if (!bTargeting&&!bShiftKeyDown)
	{
		APawn* ControllerPawn = GetPawn();
		if (UNavigationPath* NavPath=UNavigationSystemV1::FindPathToLocationSynchronously(this,ControllerPawn->GetActorLocation(),CachedDestination))
		{
			Spline->ClearSplinePoints();
			for (const FVector& PointLoc:NavPath->PathPoints)
			{
				Spline->AddSplinePoint(PointLoc,ESplineCoordinateSpace::World);
			}
			
			if (NavPath->PathPoints.Num()>0)
			{
				CachedDestination=NavPath->PathPoints[NavPath->PathPoints.Num()-1];
				bAutoRunning=true;
			}
			if (GetASC() && !GetASC()->HasMatchingGameplayTag(FBaseGameplayTags::Get().Player_Block_InputPressed))
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ClickNiagaraSystem, CachedDestination);
			}
		}
		bTargeting=false;
		FollowTime=0.f;
	}
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag Tag)
{
	if (!Tag.MatchesTagExact(FBaseGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(Tag);
		}
		return;
	}
	if (bTargeting||bShiftKeyDown)
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(Tag);
		}
	}
	else
	{
		FollowTime+=GetWorld()->GetDeltaSeconds();
		FHitResult HitResult;
		if (GetHitResultUnderCursor(ECC_Visibility,false,HitResult))
		{
			CachedDestination=HitResult.Location;
		}
		if (APawn* ControlledPawn = GetPawn())
		{
			const FVector WorldDirection=(CachedDestination-ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection);
		}
	}
}

UBaseAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if (AbilitySystemComponent==nullptr)
	{
		AbilitySystemComponent = Cast<UBaseAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn()));
	}
	return AbilitySystemComponent;
}


void AAuraPlayerController::CursorTrace()
{

	if (GetASC() && GetASC()->HasMatchingGameplayTag(FBaseGameplayTags::Get().Player_Block_CursorTrace))
	{
		if (LastActor) LastActor->UnHiglightActor();
		if (CurrentActor) CurrentActor->UnHiglightActor();
		LastActor = nullptr;
		CurrentActor = nullptr;
		return;
	}
	FHitResult HitResult;
	GetHitResultUnderCursor(ECC_Visibility, true, HitResult);
	LastActor=CurrentActor;
	CurrentActor=HitResult.GetActor();
    /**
	* Line trace from cursor. There are several scenarios:
	* A. LastActor is null && ThisActor is null
	*- Do nothing
	*B. LastActor is null && ThisActor is valid
	*- Highlight ThisActor
	* C. LastActor is valid && ThisActor is null
	*- UnHighlight LastActor
	*D. Both actors are valid, but LastActor != ThisActor
	*- UnHighlight LastActor, and Highlight ThisActor
	*E. Both actors are valid, and are the same actor
	*- Do nothing
	*/
	if (LastActor==nullptr)
	{
		if (CurrentActor!=nullptr)
		{
			CurrentActor->HiglightActor();
		}
		else
		{
			//do nothing
		}
	}
	else
	{
		if (CurrentActor==nullptr)
		{
			LastActor->UnHiglightActor();
		}
		else
		{
			if (CurrentActor!=LastActor)
			{
				LastActor->UnHiglightActor();
				CurrentActor->HiglightActor();
			}
			else
			{
				//do nothing
			}
		}
	}
}



void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraContext);
	
	UEnhancedInputLocalPlayerSubsystem *Subsystem=ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	
	if (Subsystem)
	{Subsystem->AddMappingContext(AuraContext,0);}

	bShowMouseCursor = true;
	DefaultMouseCursor=EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
	
	
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UAuraInputComponent* AuraInputComponent=CastChecked<UAuraInputComponent>(InputComponent);
	AuraInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this,&AAuraPlayerController::Move);
	AuraInputComponent->BindAction(ShiftAction,ETriggerEvent::Started,this,&AAuraPlayerController::ShiftKeyPressed);
	AuraInputComponent->BindAction(ShiftAction,ETriggerEvent::Completed,this,&AAuraPlayerController::ShiftKeyReleased);
	AuraInputComponent->BindAbilityActions(InputConfig,this,&ThisClass::AbilityInputTagPressed,&ThisClass::AbilityInputTagReleased,&ThisClass::AbilityInputTagHeld);
    
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FBaseGameplayTags::Get().Player_Block_InputPressed))
	{
		return;
	}
	const FVector2D InputAxisVector=InputActionValue.Get<FVector2D>();
	const FRotator Rotation=GetControlRotation();
	const FRotator YawRotation(0.f,Rotation.Yaw,0.f);

	const FVector ForwardDirection=FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection=FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn=GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection,InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection,InputAxisVector.X);
	}
	
}

