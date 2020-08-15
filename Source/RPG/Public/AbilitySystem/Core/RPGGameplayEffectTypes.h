// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "AbilitySystem/RPGGameplayAbility.h"
#include "UObject/NoExportTypes.h"
#include "RPGGameplayEffectTypes.generated.h"




/**
* Data structure that stores an instigator and related data, such as positions and targets
* Games can subclass this structure and add game-specific information
* It is passed throughout effect execution so it is a great place to track transient information about an execution
*/
USTRUCT()
struct RPG_API FRPGGameplayEffectContext : public FGameplayEffectContext
{
    GENERATED_USTRUCT_BODY()

public:

    virtual FGameplayAbilityTargetDataHandle GetTargetData()
    {
        return TargetData;
    }

    virtual void AddTargetData(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
    {
        TargetData.Append(TargetDataHandle);
    }

    /** This function will Get The Stacks of the Ability that the Context come from.
     *The function should be called when we send to the client a Effect Context through Parameters inside a GameplayCue Creation, unless the Effect Context Ability will be empty, so we will have to add that manually calling SetAbilityInEffecContext (This
     *last case is rare and is when we want to make some things happen with the Effect Context Outside the Ability and we create the Context Outside the Ability itself).
     * 
     */
    virtual int32 GetAbilityStacks();
 

    /**
    * Functions that subclasses of FGameplayEffectContext need to override
    */

    virtual UScriptStruct* GetScriptStruct() const override
    {
        return FRPGGameplayEffectContext::StaticStruct();
    }

    virtual FRPGGameplayEffectContext* Duplicate() const override
    {
        FRPGGameplayEffectContext* NewContext = new FRPGGameplayEffectContext();
        *NewContext = *this;
        NewContext->AddActors(Actors);
        if (GetHitResult())
        {
            // Does a deep copy of the hit result
            NewContext->AddHitResult(*GetHitResult(), true);
        }
        // Shallow copy of TargetData, is this okay?
        NewContext->TargetData.Append(TargetData);
        return NewContext;
    }

    virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;

    int32 ContextAbilityStacks = 0;

    
    
    protected:
    
    FGameplayAbilityTargetDataHandle TargetData;
};

template<>
struct TStructOpsTypeTraits<FRPGGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FRPGGameplayEffectContext>
{
    enum
    {
        WithNetSerializer = true,
        WithCopy = true		// Necessary so that TSharedPtr<FHitResult> Data is copied around
    };
};



