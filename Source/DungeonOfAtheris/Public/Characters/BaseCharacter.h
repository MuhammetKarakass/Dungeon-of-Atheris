// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "BaseCharacter.generated.h"

class UNiagaraSystem;
class UGameplayAbility;
class UAbilitySystemComponent;
class UAttributeSet;

UCLASS(Abstract) //Abstract means can't instance in the game
class DUNGEONOFATHERIS_API ABaseCharacter : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:

	ABaseCharacter();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override; //AbilitySystemInterface function for get ability system component
	UAttributeSet* GetAttributeSet() const {return AttributeSet;}

	//Combat
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual void Die() override;
	UFUNCTION(NetMulticast,Reliable)
	virtual void MulticastHandleDeath();
	virtual FVector GetSocketLocation_Implementation(const FGameplayTag& MontageTag) override;
	virtual bool IsDead_Implementation() const override;
	virtual AActor* GetAvatar_Implementation() override;
	virtual TArray<FTaggedMontage> GetAttackMontages_Implementation() override;
	virtual UNiagaraSystem* GetBloodEffect_Implementation() override;
	virtual FTaggedMontage GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag) override;
	virtual int32 GetMinionCount_Implementation() override;
	virtual void SetMinionCount_Implementation(int32 Amount) override;
	virtual ECharacterClass GetCharacterClass_Implementation() override;
	//Combat

	UPROPERTY(EditAnywhere,Category="Combat")
	TArray<FTaggedMontage> AttackTaggedMontages;

	UPROPERTY(Editanywhere,Category="Combat")
    TMap<FGameplayTag,FName> TagToSocket;

	UPROPERTY(Editanywhere,Category="Combat")
    TObjectPtr<UNiagaraSystem> BloodEffect;

	UPROPERTY(Editanywhere,Category="Combat")
	TObjectPtr<USoundBase> DeathSound;

protected:

	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo();

	void ApplyEffectToSelf(TSubclassOf<class UGameplayEffect> Effect, float level) const;
	virtual void InitDefaultAttributes() const;

	void AddCharacterAbilities();
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	// UPROPERTY(EditAnywhere, Category = "Combat")
	// FName WeaponTipSocketName;
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(BlueprintReadOnly,EditAnywhere,category="Attributes")
	TSubclassOf<class UGameplayEffect> DefaultPrimaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<class UGameplayEffect> DefaultSecondaryAttributes;

	UPROPERTY(BlueprintReadOnly,EditAnywhere, Category = "Attributes")
	TSubclassOf<class UGameplayEffect> DefaultVitalAttributes;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Character Class Defaults")
	ECharacterClass CharacterClass=ECharacterClass::Warrior;
	
	int32 MinionCount=0;

	//Dissolve Effects

	void Dissolve();

	UFUNCTION(BlueprintImplementableEvent)
	void MeshStartDissolveTimer(UMaterialInstanceDynamic* DynamicMaterialDynamic);

	UFUNCTION(BlueprintImplementableEvent)
	void WeaponStartDissolveTimer(UMaterialInstanceDynamic* DynamicMaterialDynamic);

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> MeshDissolveMaterialInstance;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;
private:
	UPROPERTY(EditAnywhere, Category="Abilites")
	TArray<TSubclassOf<UGameplayAbility>> Abilities;

	UPROPERTY(EditAnywhere, Category="Abilites")
	TArray<TSubclassOf<UGameplayAbility>> StartupPassiveAbilities;
	
	UPROPERTY(EditDefaultsOnly,Category="Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;

	bool bDead=false;
};
