// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG/Public/AbilitySystem/RPGTargetType.h"

bool URPGTargetType::GetTargets_Implementation(ARPGCharacterBase* TargetingPlayer, AActor* Instigator,
    FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const
{
    return OutActors.Num() > 0;
}





bool URPGTargetType_UseOwner::GetTargets_Implementation(ARPGCharacterBase* TargetingPlayer, AActor* Instigator,
    FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const
{
    OutActors.Add(Instigator);

    return OutActors.Num() > 0;
}

bool URPGTargetType_UseEventData::GetTargets_Implementation(ARPGCharacterBase* TargetingPlayer, AActor* Instigator,
    FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const
{
    const FHitResult* FoundHitResult = EventData.ContextHandle.GetHitResult();

    if(FoundHitResult)
    {
        OutHitResults.Add(*FoundHitResult);
    }else if (EventData.Target)
    {
        OutActors.Add(const_cast<AActor*>(EventData.Target));
    }

    return OutActors.Num() > 0;
}
