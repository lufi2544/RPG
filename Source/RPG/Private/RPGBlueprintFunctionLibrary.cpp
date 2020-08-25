// Fill out your copyright notice in the Description page of Project Settings.

#include "RPG/Public/RPGBlueprintFunctionLibrary.h"


#include "AbilitySystem/Core/RPGGameplayEffectTypes.h"
#include "Character/RPGHeroCharacter.h"
#include "Kismet/GameplayStatics.h"
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

    // I have used the Old School for loop to avoid creating a copy of the Data.

    // We itirate the TargetDataHandle Data and apply the Effec Container Specs from the ContainerSpec
    
    for(int32 TargetDataIdx = 0 ; TargetDataIdx < TargetDataHandle.Data.Num() ; ++TargetDataIdx)
    {
        // IT IS VITAL TO CHECK IF THE TARGET DATA IS VALID
        if (TargetDataHandle.Data[TargetDataIdx].IsValid())
        {
            for (int32 EffectSpecIdx = 0 ; EffectSpecIdx < ContainerSpec.TargetGameplayEffectSpecsHandle.Num(); ++EffectSpecIdx)
            {
                TargetDataHandle.Data[TargetDataIdx]->ApplyGameplayEffectSpec(*ContainerSpec.TargetGameplayEffectSpecsHandle[EffectSpecIdx].Data);
            }
        }
    }
    
}

int32 URPGBlueprintFunctionLibrary::GetAbilityStacksFromContext(FGameplayEffectContextHandle EffectContextHandle)
{
    FRPGGameplayEffectContext* RPGContext = static_cast<FRPGGameplayEffectContext*>(EffectContextHandle.Get());
    int32 iReturnStacks = 0;

    if (RPGContext)
    {
        iReturnStacks = RPGContext->ContextAbilityStacks;
    }

    return iReturnStacks;
}

FGameplayEffectContextHandle& URPGBlueprintFunctionLibrary::SetAbilityInsideEffectContext(URPGGameplayAbility* Ability, FGameplayEffectContextHandle& ContextHandle)
{
    
    FRPGGameplayEffectContext* RPGContext = static_cast<FRPGGameplayEffectContext*>(ContextHandle.Get());
    
    if (Ability)
    {
        RPGContext->SetAbility(Ability);
    }

    return ContextHandle;
}

FGameplayEffectContextHandle& URPGBlueprintFunctionLibrary::SetAbilityStacksOnEffectContext(FGameplayEffectContextHandle EffectContexthandle, int32 InStacks)
{
    FRPGGameplayEffectContext* RPGContext = static_cast<FRPGGameplayEffectContext*>(EffectContexthandle.Get());

    if (RPGContext)
    {
        RPGContext->ContextAbilityStacks = InStacks;
    }

    return EffectContexthandle;
}

void URPGBlueprintFunctionLibrary::AddHitResultToSpecHandleFromTargetData(
    TSharedPtr<FGameplayAbilityTargetData> TargetDatSharedPtr, FGameplayEffectSpecHandle& SpecHandle)
{

            if (TargetDatSharedPtr->HasHitResult())
            {
                FHitResult HitResult;

                HitResult = *TargetDatSharedPtr->GetHitResult();

                SpecHandle.Data->GetContext().AddHitResult(HitResult);
            }
        
    

    
}


bool URPGBlueprintFunctionLibrary::IsPlayerAlly(ARPGCharacterBase* Player, ARPGCharacterBase* Other)
{
    bool bSuccess = false;
    if (Player && Other)
    {
        ARPGPlayerState* MainPlayerState = nullptr;
        ARPGPlayerState* OtherPlayerState = nullptr;

        MainPlayerState =Cast<ARPGPlayerState>( Player->GetPlayerState());
        OtherPlayerState = Cast<ARPGPlayerState>( Other->GetPlayerState());


        if (MainPlayerState && OtherPlayerState)
        {
            bSuccess = (MainPlayerState->GetTeam() == OtherPlayerState->GetTeam() );
        }

        
    }

    return  bSuccess;
}

    

