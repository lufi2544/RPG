// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "RPGPlayerState.h"
#include "RPGPlayerController.h"
#include "AbilitySystemInterface.h"
#include "RPG/RPG.h"
#include "RPGCharacterBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterDieDelegate , ARPGCharacterBase*, Character);

class URPGAttributeSetBase;
class URPGGameplayAbility;




//TODO add the hit reaction logic for the characters.

// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterBaseHitReactDelegate, );




UCLASS()
class RPG_API ARPGCharacterBase : public ACharacter , public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARPGCharacterBase(const class FObjectInitializer& ObjectInitializer);

	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	/** Getters for the Character Attributes. */

	/** Character Health Amount. */
	UFUNCTION(BlueprintPure , Category = "RPG|CharacterBase|Attributes")
	float GetHealth() const;

	/** Character Max Health. We will normally set this amount for Health on Respawn. */
	UFUNCTION(BlueprintPure , Category = "RPG|CharacterBase|Attributes")
	float GetMaxHealth() const;

	/** Character Movement Speed. */
	UFUNCTION(BlueprintPure , Category = "RPG|CharacterBase|Attributes")
	float GetMoveSpeed() const;

	/** The Character Level. */
	UFUNCTION(BlueprintPure , Category = "RPG|CharacterBase|Attributes")
	float GetCharacterLevel() const;

	/** Returns if the Character is Alive. */
	UFUNCTION(BlueprintPure , Category = "RPG|CharacterBase|State")
	virtual bool IsAlive() const;

protected:
	// Called when the game starts or when spawned
    virtual void BeginPlay() override;

	/*This function will iniitialize the Attributes for the character.We
	*check if there is any attributes to Initialize on the Attributes Variable as well.*/
	virtual void InitializeAttributes();

	/** This Function will initialize the abiilities of the character. */
	virtual void InitializeAbilities();

	/** Virtual function used to set the Attribute to Max value  */
	virtual void SetHealth(float fNewHealth);

	//The Main Name of the Character
	UPROPERTY(BlueprintReadOnly ,EditAnywhere, Category= "RPG|CharacterBase")
	FText CharacterName;

	//The Character Initial Attributes
	UPROPERTY(BlueprintReadOnly ,EditAnywhere, Category = "RPG|CharacterBase")
	TSubclassOf<UGameplayEffect>CharacterInitialAttributes;

	//These are the character Abilities.
	UPROPERTY(BlueprintReadOnly ,EditAnywhere, Category = "RPG|CharacterBase")
	TArray<TSubclassOf<URPGGameplayAbility>>CharacterAbilities;

	/* I have Opted to make a TWeakPtr in Stead of a common UPROPERTY in this project because I think is more correct to have the
	 * Ability System Component and The Attribute Set on the Player State and the from there just point to that variables.
	 */

	TWeakObjectPtr<class URPGAbilitySystemComponent>AbilitySystemComponent;

	TWeakObjectPtr<class URPGAttributeSetBase>AttributeSet;
	
	

FOnCharacterDieDelegate OnCharacterDie;


};
