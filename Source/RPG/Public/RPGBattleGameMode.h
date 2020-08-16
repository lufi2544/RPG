// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPG/RPGGameMode.h"
#include "RPGStateMachine/Public/RPGBattleMachine.h"
#include "RPGBattleGameMode.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API ARPGBattleGameMode : public ARPGGameMode
{
	GENERATED_BODY()


	public:

	/** This function will initialize the Battle Logic. */
	UFUNCTION(BlueprintCallable, Category = "RPG|BattleManager")
	void StartBattle();

	/** This function will finish the Battle Logic. */
	UFUNCTION(BlueprintCallable , Category = "RPG|BattleManager")
	void FinishBattle();

	protected:

	/** This is a reference to a Battle Machine for managing the BattleState. */
	URPGBattleMachine* BattleMachine ;

	
	
};
