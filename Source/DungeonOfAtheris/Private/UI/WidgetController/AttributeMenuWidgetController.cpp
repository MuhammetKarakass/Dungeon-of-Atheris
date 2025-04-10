// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include <utility>

#include "BaseGameplayTags.h"
#include "AbilitySystem/BaseAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	UBaseAttributeSet* AS=CastChecked<UBaseAttributeSet>(AttributeSet);

	for (auto& Pair: AS->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
        [this,Pair,AS](const FOnAttributeChangeData& Data)
        {
	        FAuraAttributeInfo Info=AttributeInfo->FindAttributeInfoForTag(Pair.Key);
        	Info.AttributeValue=Pair.Value().GetNumericValue(AS);
        	AttributeInfoDelegate.Broadcast(Info);
        }
		);
	}
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	UBaseAttributeSet* AS=CastChecked<UBaseAttributeSet>(AttributeSet);
	check(AttributeInfo);

	// FAuraAttributeInfo Info=AttributeInfo->FindAttributeInfoForTag(FBaseGameplayTags::Get().Attribute_Primary_Vigor);
	// Info.AttributeValue=AS->GetVigor();
	// AttributeInfoDelegate.Broadcast(Info);

	for (auto& Pair : AS->TagsToAttributes)
	{
		FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Pair.Key);
		Info.AttributeValue = Pair.Value().GetNumericValue(AS);
		AttributeInfoDelegate.Broadcast(Info);
	}
}
