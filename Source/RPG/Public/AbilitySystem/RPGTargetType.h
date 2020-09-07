// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetDataFilter.h"
#include "Character/RPGCharacterBase.h"
#include "RPG/RPG.h"
#include "RPG/Public/Character/RPGPlayerState.h"
#include "RPG/Public/Character/RPGHeroCharacter.h"
#include "UObject/NoExportTypes.h"
#include "RPGTargetType.generated.h"



USTRUCT(BlueprintType)
struct RPG_API FRPGGameplayTargetDataFilter : public FGameplayTargetDataFilter
{
	GENERATED_USTRUCT_BODY()

	virtual ~FRPGGameplayTargetDataFilter()
	{}

	//This will be The Instigator Team, Will be plugged in by the BP Version of the Filer.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = true), Category = Filter)
	ERPGTeam Team;

	virtual bool FilterPassesForActor(const AActor* ActorToBeFiltered) const override
	{

		Super::FilterPassesForActor(ActorToBeFiltered);
		
		const ARPGCharacterBase* TargetHero = Cast<ARPGCharacterBase>(ActorToBeFiltered);
		const ARPGCharacterBase* Player = Cast<ARPGCharacterBase>(SelfActor);

		ARPGPlayerState* TargetingActorPS = Cast<ARPGPlayerState>(TargetHero->GetPlayerState());

		if (TargetingActorPS)
		{
			if (Player != TargetHero)
			{
				if (Team != TargetingActorPS->GetTeam())
				{
					return true;
				}
			}
		}

		return false;
	}
	
};

USTRUCT(BlueprintType)
struct RPG_API FRPGGameplayTargetDataFilterHandle 
{
	GENERATED_USTRUCT_BODY()

	TSharedPtr<FRPGGameplayTargetDataFilter> DataFilter;
	
};




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
