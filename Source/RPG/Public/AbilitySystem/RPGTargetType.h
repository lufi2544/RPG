// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/RPGCharacterBase.h"
#include "UObject/NoExportTypes.h"
#include "RPGTargetType.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, meta=(ShowWorldContextPin))
class RPG_API URPGTargetType : public UObject
{
	GENERATED_BODY()
public:
	
	URPGTargetType(){}

	/** Called to determine targets to apply gameplay effects to */
	UFUNCTION(BlueprintNativeEvent)
	bool GetTargets(ARPGCharacterBase* TargetingPlayer,AActor* Instigator , FGameplayEventData EventData , TArray<FHitResult>& OutHitResults, TArray<AActor*>&OutActors ) const ;

	
	
	
};


UCLASS(NotBlueprintable)
class RPG_API URPGTargetType_UseOwner : public URPGTargetType
{
	GENERATED_BODY()
	public:
	
	URPGTargetType_UseOwner(){}

    /** Called to determine targets to apply gameplay effects to */
   virtual bool GetTargets_Implementation(ARPGCharacterBase* TargetingPlayer,AActor* Instigator , FGameplayEventData EventData , TArray<FHitResult>& OutHitResults, TArray<AActor*>&OutActors ) const override;

	
	
	
};

UCLASS(NotBlueprintable)
class RPG_API URPGTargetType_UseEventData : public URPGTargetType
{
	GENERATED_BODY()
	public:
	
	URPGTargetType_UseEventData(){}

    /** Called to determine targets to apply gameplay effects to */
    virtual bool GetTargets_Implementation(ARPGCharacterBase* TargetingPlayer,AActor* Instigator , FGameplayEventData EventData , TArray<FHitResult>& OutHitResults, TArray<AActor*>&OutActors ) const override ;

	
	
	
};
