// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPG/Public/AbilitySystem/RPGAbilityTypes.h"
#include "Abilities/GameplayAbility.h"
#include "RPG/RPG.h"
#include "GameplayTagContainer.h"
#include "RPGGameplayAbility.generated.h"

class UAnimMontage;
class UGameplayEffect;


/**
 * 
 */
UCLASS()
class RPG_API URPGGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:

	URPGGameplayAbility();

	// Tells an ability to activate immediately when its granted. Used for passive abilities and abilites forced on others.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RPG|Ability")
    bool ActivateAbilityOnGranted = false;
	
	// Abilities with this set will automatically activate when the input is pressed
	UPROPERTY(BlueprintReadOnly , EditAnywhere , Category = "RPG|Ability")
	ERPGbilityInputID AbilityInputID = ERPGbilityInputID::None;

	// Value to associate an ability with an slot without tying it to an automatically activated input.
	// Passive abilities won't be tied to an input so we need a way to generically associate abilities with slots.
	UPROPERTY(BlueprintReadOnly , EditAnywhere , Category = "RPG|Ability")
	ERPGbilityInputID AbilityID = ERPGbilityInputID::None;

	//We will set the Ability Stacks acording to this montages lenght.
	UPROPERTY(BlueprintReadOnly , EditAnywhere , Category = "RPG|Ability")
	TArray<UAnimMontage*> CharacterMontagesToPlay;

	//True if the Ability Max Stacks are greater than 0, too. Mark this as true if you want to make an ability that can be recasted for an amount of time before
	//the ability goes to cooldown.
	UPROPERTY(BlueprintReadOnly , EditAnywhere , Category = "RPG|Ability|Stacks")
	uint8 bIsStackable : 1;

	//True if the Ability will have have combos to play by the character, in that case we set the Ability Max Stacks to the amount of possible attacks.
	UPROPERTY(BlueprintReadOnly , EditAnywhere , Category = "RPG|Ability|Stacks")
    uint8 bIsComboAbility : 1;

	//Wether the Ability Has ended or not.
    UPROPERTY(BlueprintReadWrite , Category= "RPG|Ability")
	uint8 bHasEnded : 1;

	//Window time that the player has to recast the ability if the ability is stackable.
	UPROPERTY(BlueprintReadWrite , EditAnywhere ,Category = "RPG|Ability|Stacks")
	float RecastTime = 0.0f;
	
	// These are the stacks of the Ability, you can custom this property in order for the ability to work with a maximum amount of recast
	//the ability in a certain amount of time.(This property will be tied to the IsStackable Property.)
	UPROPERTY(BlueprintReadWrite, Category = "RPG|Ability|Stacks")
	int32 AbilityStacks = 0;

	//Max Ability Stacks.The property amount will be set ->(if the Ability IsStackable, AbilityMaxStacks will be the number set on the Ability BP) ,
	//	(if IsComboAbility the Ability Max Stacks will be the montages to play number.)
	UPROPERTY(BlueprintReadWrite , EditAnywhere, Category = "RPG|Ability|Stacks")
    int32 AbilityMaxStacks = 0;

	//This is a map made of a GameplayTag and a GameplayEffectContainer that will apply all the effects to the target or the owwer later when is called.
	/**
	 * \brief This is a nice template
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RPG|Ability|Effects")
	TMap<FGameplayTag, struct FRPGGameplayEffectContainer> EffectContainerMap;

	
	// If an ability is marked as 'ActivateAbilityOnGranted', activate them immediately when given here
	// Epic's comment: Projects may want to initiate passives or do other "BeginPlay" type of logic here.
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	/** We make a Gameplay Effect Container Spec to be applied later when is needed. */
	UFUNCTION(BlueprintCallable, Category = "RPG|Ability")
	virtual FRPGGameplayEffectContainerSpec MakeEffectContainerSpec(FGameplayTag ContainerTag, const FGameplayEventData& EventData, int32 OverrideGameplayLevel = -1);


	virtual FRPGGameplayEffectContainerSpec MakeContainerSpecFromContainer(const FRPGGameplayEffectContainer& Container , const FGameplayEventData& EventData , int32 OverrideGameplayLevel = -1);

	/** We apply a certain Effect Container Spec*/
	UFUNCTION(BlueprintCallable, Category = "RPG|Ability")
	TArray<FActiveGameplayEffectHandle> ApplyEffectContainerSpec(const FRPGGameplayEffectContainerSpec& ContainerSpec);
	
	/** Does the commit atomically (consume resources, do cooldowns, etc) */
	virtual void CommitExecute(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo);

	//Sets the Ability Stacks to 0;
	UFUNCTION(BlueprintCallable , Category= "RPG|Ability")
	void ResetAbilityStacks();

	/** Applies CooldownGameplayEffect to the target */
	virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const;

};
