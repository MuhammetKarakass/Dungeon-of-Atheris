// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAssetManager.h"

#include "BaseGameplayTags.h"

UBaseAssetManager& UBaseAssetManager::Get()
{
	check(GEngine)
	UBaseAssetManager* AssetManager=Cast<UBaseAssetManager>(GEngine->AssetManager);

	return *AssetManager;
}

void UBaseAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FBaseGameplayTags::InitializeNativeGameplayTags();
}
