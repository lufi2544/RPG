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

	void BeginPlay() override;
	
	//Getter for the AbilitySystemComponent from the Interface
	class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	void GetLifetimeReplicatedProps( TArray< FLifetimeProperty > & OutLifetimeProps ) const override;
	
	//Getter for the AttributeSet Base
	class URPGAttributeSetBase* GetAttributeSetBase() const;

	/** Core Functions */

	/** Return the Turn State of the Player( If he has moved this turn.) */
	UFUNCTION(BlueprintPure , Category = "RPG|RPGPlayerState")
	bool GetPlayerTurnState();

	UFUNCTION(BlueprintCallable , Category = "RPG|RPGPlayerState")
	/** Sets if the Character has moved this turn. */
	void SetPlayerStateTurn( bool bNewState );

	/** Properties */

	
	//This Is the Inventory Component for the Player which stores all the data related to the Objects that he has on the Inventory.
	UPROPERTY(BlueprintReadOnly, Category = "RPG|PlayerState|Inventory",ReplicatedUsing = OnRep_InventoryComponent, meta=(AllowPrivateAccess="True"))
	ARPGPlayerInventoryComponent* InventoryComponent;


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

	UFUNCTION(BlueprintPure , Category = "RPG|RPGPlayerState")
	ERPGTeam GetTeam();

	UFUNCTION(BlueprintCallable, Category= "RPG|RPGPlayerState")
	void SetTeam(ERPGTeam NewTeam);

	
	void CreateInventoryComponent();
	
	protected:

	/** This is the Character Team, is used for Targeting in the Game. */
	UPROPERTY(BlueprintReadWrite,EditAnywhere, Category = "RPG|PlayerState",Replicated)
	ERPGTeam Team = ERPGTeam::Neutral;
	
	FDelegateHandle HealthChangeDelegateHandle;

	UFUNCTION()
	virtual void OnRep_InventoryComponent();

	virtual void HealthChange(const FOnAttributeChangeData& Data);

	
	
private:


	UPROPERTY()
	class URPGAbilitySystemComponent* AbilitySystemComponent;
	
	UPROPERTY()
	class URPGAttributeSetBase* AttributeSet;

	/** Turn Logic Variables */

	/** Wether the Character has finished his moves this turn. */
	uint32 bHasMoved : 1;



};


