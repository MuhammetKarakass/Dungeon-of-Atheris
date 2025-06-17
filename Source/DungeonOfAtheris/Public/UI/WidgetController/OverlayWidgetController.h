
#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/WidgetController/BaseWidgetController.h"
#include "AbilitySystem/Data/AbilityInfoNew.h"
#include "OverlayWidgetController.generated.h"

class UBaseAbilitySystemComponent;
class UBaseUserWidget;
struct FGameplayTag;
struct FOnAttributeChangeData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangeSignature,float,NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityInfoSignature, const FAuraAbilityInfoNew&, Info);

USTRUCT(BlueprintType)
struct FUIWidgetRow:public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FGameplayTag GameplayTag=FGameplayTag();

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FText Message=FText();

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<UBaseUserWidget> MessageWidget;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<UTexture2D> Image=nullptr;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature, FUIWidgetRow, Row);
/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class DUNGEONOFATHERIS_API UOverlayWidgetController : public UBaseWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FOnAttributeChangeSignature OnHealthChanged;
	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FOnAttributeChangeSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FOnAttributeChangeSignature OnManaChanged;
	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FOnAttributeChangeSignature OnMaxManaChanged;

	UPROPERTY(BlueprintAssignable,Category="GAS|Messages")
	FMessageWidgetRowSignature MessageWidgetRowDelegate;

	UPROPERTY(BlueprintAssignable,Category="GAS|Messages")
	FAbilityInfoSignature AbilityInfoDelegateNew;
	
	UPROPERTY(BlueprintAssignable,Category="GAS|XP")
	FOnAttributeChangeSignature OnPercentChangedDelegate;

	UPROPERTY(BlueprintAssignable,Category="GAS|XP")
	FOnPlayerStatChangeSignature OnPlayerLevelChangeDelegate;

protected:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Widget Data")
	TObjectPtr<UDataTable> MessageWidgetDataTable;

	// UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Widget Data")
	// TObjectPtr<UAbilityInfo> AbilityInfo;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Widget Data")
	TObjectPtr<UAbilityInfoNew> AbilityInfoNew;

	void OnInıtialieStartupAbilities(UBaseAbilitySystemComponent* AbilitySystemComponent);

	template<typename T>
	T* GetDataTableRowByTag(UDataTable* DataTable,const FGameplayTag& Tag);

	void OnXPChanged(int32 NewXP)const;
};

template <typename T>
T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable,const FGameplayTag& Tag)
{
	return DataTable->FindRow<T>(Tag.GetTagName(),TEXT(""));
}
