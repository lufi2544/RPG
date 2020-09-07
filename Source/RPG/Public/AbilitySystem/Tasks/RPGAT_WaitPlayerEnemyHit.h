// Ishak Hero.All Rights Reserved.

#pragma once

#include "AbilitySystemComponent.h"
#include "CoreMinimal.h"

#include "RPGEnemy.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "RPG/RPG.h"

#include "RPGAT_WaitPlayerEnemyHit.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHitEvent , ARPGCharacterBase* , InstigatorCharacter,ARPGCharacterBase*,HitCharacter,ERPGHitType ,HitType);

/**
 * Task that will listen for a Player Getting Hit By an enemy or an Enemy getting hit by a Player. (Commonly used to initiate a Battle).
 */
UCLASS(BlueprintType , meta = (ExposedAsyncProxy = AsyncTask))
class RPG_API URPGAT_WaitPlayerEnemyHit : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

	public:
	
	UPROPERTY(BlueprintAssignable)
	FOnHitEvent OnBattleHitEvent;

	UFUNCTION(BlueprintCallable , Category = "RPG|Tasks" , meta= (BlueprintInternalUseOnly = "true"))
	static URPGAT_WaitPlayerEnemyHit* WaitPlayerEnemyHit(UAbilitySystemComponent* PlayerASC);

	UFUNCTION(BlueprintCallable)
	void EndTask();

	protected:

	UAbilitySystemComponent* ASC;

	void OnPlayerHitCallBack(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& Spec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle);
	void OnEnemyHitCallBack(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& Spec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle);
	
};


