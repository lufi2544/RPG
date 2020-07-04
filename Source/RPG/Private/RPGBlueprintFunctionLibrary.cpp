// Fill out your copyright notice in the Description page of Project Settings.


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
        Filter.InitializeFilterContext(Actor);

        *TargetDataFilterHandle.DataFilter = Filter;
    }
    
    
    return TargetDataFilterHandle;
    
}
