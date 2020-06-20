// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG/Public/AbilitySystem/RPGAbilityTypes.h"

bool FRPGGameplayEffectContainerSpec::HasValidEffects() const
{
    return TargetGameplayEffectSpecsHandle.Num() > 0;
}

bool FRPGGameplayEffectContainerSpec::HasValidTargets() const
{
    return TargetData.Num() > 0;
}

void FRPGGameplayEffectContainerSpec::AddTargets(const TArray<FHitResult>& HitResults,
    const TArray<AActor*>& TargetActors)
{

    // We want to use the Hit Results first and then if we consider, we use the Actors array.
    if (HitResults.Num() > 0)
    {

        for(const FHitResult& HitResult : HitResults)
        {
            FGameplayAbilityTargetData_SingleTargetHit* NewData = new FGameplayAbilityTargetData_SingleTargetHit(HitResult);
            TargetData.Add(NewData);
        }
        
    }else
    {
        if (TargetActors.Num() > 0)
        {
            FGameplayAbilityTargetData_ActorArray* NewData = new FGameplayAbilityTargetData_ActorArray();
            NewData->TargetActorArray.Append(TargetActors);
            TargetData.Add(NewData);
        }
    }
}
    


