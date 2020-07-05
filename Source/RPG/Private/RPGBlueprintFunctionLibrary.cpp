// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystemBlueprintLibrary.h"
#include "RPG/Public/RPGBlueprintFunctionLibrary.h"


#include "Character/RPGHeroCharacter.h"
#include "RPG/Public/AbilitySystem/RPGAbilitySystemComponent.h"


void URPGBlueprintFunctionLibrary::AddTargetsToGameplayEffectSpec(FRPGGameplayEffectContainerSpec ContainerSpec,
    TArray<FHitResult> HitResults, TArray<AActor*> Actors)
{
    if (HitResults.Num() > 0 || Actors.Num() > 0)
    {
        ContainerSpec.AddTargets(HitResults,Actors);
    }
    
}

TArray<FActiveGameplayEffectHandle> URPGBlueprintFunctionLibrary::ApplyGameplayEffectContainerSpec(
    FRPGGameplayEffectContainerSpec ContainerSpec)
{
    
    TArray<FActiveGameplayEffectHandle>ActiveGameplayEffectHandles;

    for (const FGameplayEffectSpecHandle& SpecHandle : ContainerSpec.TargetGameplayEffectSpecsHandle)
    {
        if (SpecHandle.IsValid())
        {
            // If effect is valid, iterate list of targets and apply to all
            for (TSharedPtr<FGameplayAbilityTargetData> Data : ContainerSpec.TargetData.Data)
            {
                ActiveGameplayEffectHandles.Append(Data->ApplyGameplayEffectSpec(*SpecHandle.Data.Get()));
            }
        }
    }
    
        
   return ActiveGameplayEffectHandles;
}

 bool URPGBlueprintFunctionLibrary::DoesContainerSpecHasEffects(FRPGGameplayEffectContainerSpec ContainerSpec)
{

 

    return  ContainerSpec.HasValidEffects();
}

FRPGGameplayTargetDataFilterHandle URPGBlueprintFunctionLibrary::MakeRPGGameplayTargetDataFilterHandle(
    FRPGGameplayTargetDataFilter Filter, AActor* Actor)
{
    FRPGGameplayTargetDataFilterHandle TargetDataFilterHandle;

    if (IsValid(Actor))
    {

        FRPGGameplayTargetDataFilter* TargetDataFilter = new FRPGGameplayTargetDataFilter(Filter);

        TargetDataFilter->InitializeFilterContext(Actor);

        TargetDataFilterHandle.DataFilter = TSharedPtr<FRPGGameplayTargetDataFilter>(TargetDataFilter);
        
    }
    
    
    return TargetDataFilterHandle;
    
}

void URPGBlueprintFunctionLibrary::ApplyEffectContainerSpecToTargetsFromTargetData(FRPGGameplayEffectContainerSpec ContainerSpec, const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{

    //Get all the actors from the Target Data
    
    TArray<AActor*>	Result;
    for (int32 TargetDataIndex = 0; TargetDataIndex < TargetDataHandle.Data.Num(); ++TargetDataIndex)
    {
        if (TargetDataHandle.Data.IsValidIndex(TargetDataIndex))
        {
            const FGameplayAbilityTargetData* DataAtIndex = TargetDataHandle.Data[TargetDataIndex].Get();
            if (DataAtIndex)
            {
                TArray<TWeakObjectPtr<AActor>> WeakArray = DataAtIndex->GetActors();
                for (TWeakObjectPtr<AActor>& WeakPtr : WeakArray)
                {
                    Result.Add(WeakPtr.Get());
                }
            }
        }
    }

    // Then we apply all the effects from the Container Spec
    
    FGameplayAbilityTargetData_ActorArray* TargetData_Actors = new FGameplayAbilityTargetData_ActorArray();

    TargetData_Actors->TargetActorArray.Append(Result);
    
    for (FGameplayEffectSpecHandle SpecHandle : ContainerSpec.TargetGameplayEffectSpecsHandle)
    {
        if (SpecHandle.IsValid())
        {
            TargetData_Actors->ApplyGameplayEffectSpec(*SpecHandle.Data);
        }
    }
    
}

