// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/BaseHUD.h"

#include "UI/UserWidget/BaseUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"

UOverlayWidgetController* ABaseHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController=NewObject<UOverlayWidgetController>(this,OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallbacksToDependencies();
		return OverlayWidgetController;
	}
	return OverlayWidgetController;
}

void ABaseHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass,TEXT("Overlay Widget class is null"));
	checkf(OverlayWidgetControllerClass,TEXT("Overlay Widget Controller class is null"));

	UUserWidget *Widget=CreateWidget<UUserWidget>(GetWorld(),OverlayWidgetClass);
	OverlayWidget=Cast<UBaseUserWidget>(Widget);

	const FWidgetControllerParams WidgetControllerParams(PC,PS,ASC,AS);
	UOverlayWidgetController* WidgetController=GetOverlayWidgetController(WidgetControllerParams);
	
	OverlayWidget->SetWidgetController(WidgetController);
	WidgetController->BroadcastInitialValues();
	Widget->AddToViewport();
}
