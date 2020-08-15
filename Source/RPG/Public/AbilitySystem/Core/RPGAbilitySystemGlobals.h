// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "RPGAbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API URPGAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

	public:

	URPGAbilitySystemGlobals();

	/**
	* Cache commonly used tags here. This has the benefit of one place to set the tag FName in case tag names change and
	* the function call into URPGAbilitySystemGlobals::RPGGet() is cheaper than calling FGameplayTag::RequestGameplayTag().
	* Classes can access them by URPGAbilitySystemGlobals::RPGGet().DeadTag
	* We're not using this in this sample project (classes are manually caching in their constructors), but it's here as a reference.
	*/

	UPROPERTY()
	FGameplayTag DeadTag;

	UPROPERTY()
	FGameplayTag StunTag;

	static URPGAbilitySystemGlobals& RPGGet()
	{
		return dynamic_cast<URPGAbilitySystemGlobals&>(Get());
	}

	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;

	virtual void InitGlobalTags() override;

	
};
