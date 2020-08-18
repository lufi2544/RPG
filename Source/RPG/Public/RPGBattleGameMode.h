// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPG/RPGGameMode.h"
#include "RPGEnemy.h"
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
	UFUNCTION(BlueprintCallable, Category = "RPG|GameMode|Battle")
	void StartBattle(TArray<ARPGEnemy*>Enemies , TArray<ARPGHeroCharacter*> Allies);

	UFUNCTION(BlueprintCallable , Category = "RPG|GameMode|Battle")
	void RunBattle(EBattleMachineEndState& EndState , ERPGTeam TeamToInitiate ,  ERPGTeam& out_LastTeam);

	/** This function will finish the Battle Logic. */

	void FinishBattle(EBattleMachineEndState EndState);

	void PlayerFinishedTurn();

	void TeamFinishedTurn(ERPGTeam Team);

	protected:

	/** This is a reference to a Battle Machine for managing the BattleState. */
	URPGBattleMachine* BattleMachine ;


	/** Delegates */

	UFUNCTION(BlueprintImplementableEvent)
	void OnBattleFinished();

	UFUNCTION(BlueprintImplementableEvent)
	void OnPlayerTurnEnded();

	UFUNCTION(BlueprintImplementableEvent)
	void OnTeamTurnEnded();
	
};



