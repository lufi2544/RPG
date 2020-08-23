// Ishak Hero.All Rights Reserved.

#pragma once

#include "AbilitySystemComponent.h"
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "RPGAT_WaitPlayerEnemyHit.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnemyHit);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerHit);

/**
 * Task that will listen for a Player Getting Hit By an enemy or an Enemy getting hit by a Player. (Commonly used to initiate a Battle).
 */
UCLASS(BlueprintType , meta = (ExposedAsyncProxy = AsyncTask))
class RPG_API URPGAT_WaitPlayerEnemyHit : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

	public:
	
	UPROPERTY(BlueprintAssignable)
	FOnPlayerHit OnPlayerHit;

	UPROPERTY(BlueprintAssignable)
	FOnEnemyHit OnEnemyHit;

	UFUNCTION(BlueprintCallable , Category = "RPG|Tasks" , meta= (BlueprintInternalUseOnly = "true"))
	static URPGAT_WaitPlayerEnemyHit* WaitPlayerEnemyHit(UAbilitySystemComponent* PlayerASC);

	UFUNCTION(BlueprintCallable)
	void EndTask();

	protected:

	UAbilitySystemComponent* ASC;

	void OnPlayerHitCallBack(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& Spec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle);
	void OnEnemyHitCallBack(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& Spec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle);
	
};


