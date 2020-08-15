// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG/Public/AbilitySystem/Core/RPGAbilitySystemGlobals.h"

#include "AbilitySystem/Core/RPGGameplayEffectTypes.h"

URPGAbilitySystemGlobals::URPGAbilitySystemGlobals()
{
    
}

FGameplayEffectContext* URPGAbilitySystemGlobals::AllocGameplayEffectContext() const
{
  return new FRPGGameplayEffectContext();   
}

void URPGAbilitySystemGlobals::InitGlobalTags()
{

  Super::InitGlobalTags();


  // The Tags that are going to be used the most should be located here, so it is cheaper to access them through the Ability System Globals.
  
  DeadTag = FGameplayTag::RequestGameplayTag("State.Dead");
  
}


