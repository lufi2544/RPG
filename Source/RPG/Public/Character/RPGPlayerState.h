// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"

#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "RPGCharacterBase.h"
#include "RPGPlayerInventoryComponent.h"
#include "RPG/Public/Items/RPGEquipableItem.h"
#include "RPG/RPG.h"



#include "RPGPlayerState.generated.h"

class ARPGCharacterBase;
class URPGAttributeSetBase;
class URPGAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class RPG_API ARPGPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

		ARPGPlayerState();
	
	//Getter for the AbilitySystemComponent from the Interface
	class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	void GetLifetimeReplicatedProps( TArray< FLifetimeProperty > & OutLifetimeProps ) const override;
	
	//Getter for the AttributeSet Base
	class URPGAttributeSetBase* GetAttributeSetBase() const;

	/*
	 * Getters for the Character State(AttributeSet related)
	 */

	UFUNCTION(BlueprintPure , Category = "RPG|RPGPlayerState|Attributes")
	float GetHealth() const;

	UFUNCTION(BlueprintPure , Category = "RPG|RPGPlayerState|Attributes")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintPure , Category = "RPG|RPGPlayerState|Attributes")
	float GetMoveSpeed() const;

	UFUNCTION(BlueprintPure , Category = "RPG|RPGPlayerState|Attributes")
	float GetCharacterLevel() const;

	UFUNCTION(BlueprintPure, Category = "RPG|PlayerState|Inventory")
	ARPGPlayerInventoryComponent* GetInventoryComponent();

	UFUNCTION(BlueprintPure , Category = "RPG|RPGPlayerState|State")
	bool IsAlive() const;

private:


	UPROPERTY()
	class URPGAbilitySystemComponent* AbilitySystemComponent;
	
	UPROPERTY()
	class URPGAttributeSetBase* AttributeSet;

	//This Is the Inventory Component for the Player which stores all the data related to the Objects that he has on the Inventory.
	UPROPERTY(BlueprintReadOnly, Category = "RPG|PlayerState|Inventory",Replicated, meta=(AllowPrivateAccess="True"))
	ARPGPlayerInventoryComponent* InventoryComponent;


};


