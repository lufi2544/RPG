// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG/Public/AbilitySystem/Core/RPGGameplayEffectTypes.h"

int32 FRPGGameplayEffectContext::GetAbilityStacks()
{
    int32 ReturnStacks = 0;

    FRPGGameplayEffectContext* RPGContext = static_cast<FRPGGameplayEffectContext*>(this);
    
    const URPGGameplayAbility* RPGAbility = Cast<URPGGameplayAbility>(RPGContext->GetAbility());
   
    if (RPGAbility)
    {
        ReturnStacks = RPGAbility->AbilityStacks;
    }

    return ReturnStacks;
}

bool FRPGGameplayEffectContext::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
    return Super::NetSerialize(Ar, Map, bOutSuccess) && TargetData.NetSerialize(Ar, Map, bOutSuccess);
}
    

