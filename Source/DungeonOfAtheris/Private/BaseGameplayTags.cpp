// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGameplayTags.h"
#include "GameplayTagsManager.h"

FBaseGameplayTags FBaseGameplayTags::GameplayTags;

void FBaseGameplayTags::InitializeNativeGameplayTags()
{

	//Primary Attributes Tags
	GameplayTags.Attribute_Primary_Strength=UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Strength"),
		FString("Increases physical damage"));

	GameplayTags.Attribute_Primary_Intelligence=UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Intelligence"),
		FString("Increases magical damage"));

	GameplayTags.Attribute_Primary_Resilience=UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Resilience"),
		FString("Increases Armor and Armor Penetration"));

	GameplayTags.Attribute_Primary_Vigor=UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Vigor"),
		FString("Increases Health"));


	//Secondary Attributes Tags
	GameplayTags.Attribute_Secondary_Armor=UGameplayTagsManager::Get().AddNativeGameplayTag
	(FName("Attributes.Secondary.Armor"),
	FString("Reduces damage taken, improves Block Chance"));
	
	GameplayTags.Attribute_Secondary_ArmorPenetration=UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName("Attributes.Secondary.ArmorPenetration"),
	FString("Ignores Percentage of enemy Armor, Increases Critical Hit Chance"));

	GameplayTags.Attribute_Secondary_BlockChance=UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.BlockChance"),
		FString("Chance to cut incoming damage in half"));

	GameplayTags.Attribute_Secondary_CriticalHitChance=UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitChance"),
		FString("Chance to double damage plus to critical hit bonus"));

	GameplayTags.Attribute_Secondary_CriticalHitDamage=UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitDamage"),
		FString("Bonus to damage plus to critical hit bonus"));

	GameplayTags.Attribute_Secondary_CriticalHitResistance=UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitResistance"),
		FString("Reduces Critical Hit Chance of attacking enemies"));

	GameplayTags.Attribute_Secondary_HealthRegeneration=UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.HealthRegeneration"),
		FString("Amount of Health regenerated every 1 second"));

	GameplayTags.Attribute_Secondary_ManaRegeneration=UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.ManaRegeneration"),
		FString("Amount of Mana regenerated every 1 second"));

	GameplayTags.Attribute_Secondary_MaxHealth=UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxHealth"),
		FString("Maximum amount of Health"));

	GameplayTags.Attribute_Secondary_MaxMana=UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxMana"),
		FString("Maximum amount of Mana"));
	
	//Input Tags
	GameplayTags.InputTag_LMB=UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.LMB"),
		FString("Input tag for Left Mouse Button"));

	GameplayTags.InputTag_RMB=UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.RMB"),
		FString("Input tag for Right Mouse Button"));

	GameplayTags.InputTag_1=UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.1"),
		FString("Input tag for 1 Key"));

	GameplayTags.InputTag_2=UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.2"),
		FString("Input tag for 2 Key"));

	GameplayTags.InputTag_3=UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.3"),
		FString("Input tag for 3 Key"));

	GameplayTags.InputTag_4=UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.4"),
		FString("Input tag for 4 key"));

	//Hit React
	GameplayTags.Effects_HitReact=UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effects.HitReact"),
		FString("Tag granted when hit reacting"));
	
	//DamageTypes
	GameplayTags.Damage=UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage"),
		FString("Damage"));

	GameplayTags.Damage_Arcane=UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Arcane"),
		FString("Arcane Damage Type"));
	
	GameplayTags.Damage_Fire=UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Fire"),
		FString("Fire Damage Type"));

	GameplayTags.Damage_Lightning=UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Lightning"),
		FString("Lightning Damage Type"));

	GameplayTags.Damage_Physical=UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage.Physical"),
		FString("Physical Damage Type"));

	GameplayTags.Attribute_Resistance_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Resistance.Arcane"),
		FString("Resistance to Arcane damage")
		);
	GameplayTags.Attribute_Resistance_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Resistance.Fire"),
		FString("Resistance to Fire damage")
		);
	GameplayTags.Attribute_Resistance_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Resistance.Lightning"),
		FString("Resistance to Lightning damage")
		);
	GameplayTags.Attribute_Resistance_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Resistance.Physical"),
		FString("Resistance to Physical damage")
		);

//Abilities	
	GameplayTags.Abilities_Attack=UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Attack"),
		FString("Ability to Attack"));

	GameplayTags.Abilities_Summon=UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Summon"),
		FString("Ability to Summon"));

	GameplayTags.Abilities_Fire_FireBolt=UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Fire.FireBolt"),
		FString("FireBolt Ability Tag"));
	
	
//Combat Sockets

	GameplayTags.CombatSocket_Weapon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CombatSocket.Weapon"),
		FString("Weapon")
		);

	GameplayTags.CombatSocket_RightHand = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CombatSocket.RightHand"),
		FString("Right Hand")
		);
	
	GameplayTags.CombatSocket_LeftHand = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CombatSocket.LeftHand"),
		FString("Left Hand")
		);

	GameplayTags.CombatSocket_Tail=UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CombatSocket.Tail"),
		FString("Tail"));
	
//Attack Montages
	
	GameplayTags.Montage_Attack_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.1"),
		FString("Montage Attack 1")
		);

	GameplayTags.Montage_Attack_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.2"),
		FString("Montage Attack 2")
		);

	GameplayTags.Montage_Attack_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.3"),
		FString("Montage Attack 3")
		);

	GameplayTags.Montage_Attack_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.4"),
		FString("Montage Attack 4")
		);

	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Fire,GameplayTags.Attribute_Resistance_Fire);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Arcane,GameplayTags.Attribute_Resistance_Arcane);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Lightning,GameplayTags.Attribute_Resistance_Lightning);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Physical,GameplayTags.Attribute_Resistance_Physical);

	
}
