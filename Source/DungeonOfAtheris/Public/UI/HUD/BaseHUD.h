// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BaseHUD.generated.h"
class UAttributeMenuWidgetController;
class UAttributeSet;
class UOverlayWidgetController;
class UBaseUserWidget;
class UAbilitySystemComponent;
struct FWidgetControllerParams;
/**
 * 
 */
UCLASS()
class DUNGEONOFATHERIS_API ABaseHUD : public AHUD
{
	GENERATED_BODY()

public:
	

	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);
	UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams);

	void InitOverlay(APlayerController* PC,APlayerState* PS, UAbilitySystemComponent* ASC,UAttributeSet* AS);
	
protected:

private:

	UPROPERTY()
	TObjectPtr<UBaseUserWidget> OverlayWidget;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UBaseUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;
};
