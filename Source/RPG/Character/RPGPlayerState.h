// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"

#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "RPGCharacterBase.h"
#include "RPG/RPGAbilitySystemComponent.h"
#include "RPG/AbilitySystem/RPGAttributeSetBase.h"


#include "RPGPlayerState.generated.h"




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

	//Getter for the AttributeSet Base
	class URPGAttributeSetBase* GetAttributeSetBase() const;

	/*
	 * Getters for the Character State(AttributeSet related)
	 */

	float GetHealth() const;

	float GetMaxHealth() const;

	float GetMoveSpeed() const;

protected:

	void PostInitializeComponents() override;

	UPROPERTY()
	class URPGAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	class URPGAttributeSetBase* AttributeSet;

	
};
