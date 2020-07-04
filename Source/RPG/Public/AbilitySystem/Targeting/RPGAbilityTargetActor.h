// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "AbilitySystem/RPGTargetType.h"
#include "Kismet/KismetSystemLibrary.h"


#include "RPGAbilityTargetActor.generated.h"


class UGameplayAblity;

/**
 * 
 */
UCLASS()
class RPG_API ARPGAbilityTargetActor : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()
	
	public:

	UPROPERTY(BlueprintReadWrite, Replicated, meta = (ExposeOnSpawn = true), Category = Targeting)
    FRPGGameplayTargetDataFilterHandle RPGFilter;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true), Category = Targeting)
	 float Radius;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true), Category = Targeting)
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true), Category = Targeting)
	TArray<AActor*>IgnoreActors;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true), Category = Targeting)
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebug ;


	void GetLifetimeReplicatedProps( TArray< FLifetimeProperty > & OutLifetimeProps ) const override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


	virtual void SphereTraceWithFilter(TArray<FHitResult>&Out_HitResults , const FRPGGameplayTargetDataFilterHandle FilterHandle , const FVector& Start , const FVector& End);


	virtual void StartTargeting(UGameplayAbility* Ability) override;

	virtual void ConfirmTargetingAndContinue() override;

	virtual void Tick(float DeltaSeconds) override;



	protected:

	FGameplayAbilityTargetDataHandle MakeTargetData(const TArray<FHitResult>&HitResults) const;
	
};
