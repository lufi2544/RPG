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

	UFUNCTION(BlueprintCallable, Category = "RPG|BattleManager")
	void StartBattle();

	protected:

	URPGBattleMachine* BattleMachine ;
	
};
