// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPG/RPGGameMode.h"
#include "RPGEnemy.h"
#include "RPG/Public/Character/RPGHeroCharacter.h"
#include "RPGStateMachine/Public/RPGBattleMachine.h"
#include "RPGBattleGameMode.generated.h"




UENUM(BlueprintType)
enum class EBattleMachineEndState : uint8
{

	Continue,
	
	// There is no more enemies on the Map, all deffeted or they have may  escaped.
	OutOfEnemies,

    // there is no more allies on the Map, all defeted or they have may escaped
    OutOfAllies,

    // Allies Escaped
    EnemiesEscaped,

    // Enemies Escaped
    AlliesEscaped,

    None


};


UENUM(BlueprintType)
enum class EBattleMachineState  : uint8
{

	// We continue Branching
	Accepted,

    // The State got rejected by any means
    Rejected,

    Idle
	
};




DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFinishBattle , EBattleMachineEndState , BattleMachineEndState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTeamTurnFinished , ERPGTeam, FinishedTeam );
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerFinishedTurn);



/**
 * 
 */
UCLASS()
class RPG_API ARPGBattleGameMode : public ARPGGameMode
{
	GENERATED_BODY()


	public:

	
	

	/** The Main Player Controller controlled by the Player. */
	ARPGPlayerController* MainPlayerController;

	/** Ally Charactes that has entered the Battle. */
	TArray<ARPGHeroCharacter*> AllyCharacters;

	/** Enemy Characters that has entered the Battle. */
	TArray<ARPGHeroCharacter*> EnemyCharacters;

	/** --------------------------------------------------------------------------------------------------------- */

	/** This function will initialize the Battle Logic. */
	UFUNCTION(BlueprintCallable, Category = "RPG|GameMode|Battle")
	void StartBattle(TArray<ARPGHeroCharacter*>Enemies , TArray<ARPGHeroCharacter*> Allies , ARPGPlayerController* PC);

	UFUNCTION(BlueprintCallable , Category = "RPG|GameMode|Battle")
	void RunBattle(EBattleMachineEndState& EndState , ERPGTeam TeamToInitiate ,  ERPGTeam& out_LastTeam);

	/** True if the Battle has Started. */
	UFUNCTION(BlueprintPure , Category = "RPG|GameMode|Battle")
	bool GetBattleState();



	/** The Core Battle State Functions */

	
	/** Function that will transport the Player to the BattleMap to Start the Battle.
	 *
	 * @param Enemies The Enemies that have been Spawned on the Map.
	 * @param Allies The Ally Charactes who are on the map. 
	 */
	void StartBattle(TArray<ARPGHeroCharacter*>&Enemies , TArray<ARPGHeroCharacter*>& Allies);

	/** Runs All the Battle State Machine */
	void RunBattleStateMachine(EBattleMachineEndState& EndState, ERPGTeam TeamToInitiate , ERPGTeam& out__LastTeam);

	/** Stops the State Machine */
	void StopBattleStateMachine(EBattleMachineEndState &EndState);

	/** Function that checks the State of the Characters in general and that returns a State acording to that.
	 *
	 *  	Accepted, we can continue branching (The turns may continue).
	 *  	
	 *  	Rejected, Allies have escaped, Allies have been defeated .
	 *  			  Enemies have escaped, Enemies have been defeated.
	 *
	 * The BattleMachineEndState passed by reference will tell us the reason of being rejected.
	 */
	EBattleMachineState TryCheckBattleState(EBattleMachineEndState& BattleMachineEndState);

	/** Checks if the Hero Charactes are alive.
	 *
	 * @return True if there is at least one player alive.
	 */
	bool CheckPlayerStates();

	/** Checks if the Enemies are still Alive. Same Functionaluty as CheckPlayersStates()
	 *
	 * @return True if there is at least 1 enemy alive.
	 */
	bool CheckEnemiesState();



	/** Turn Logic */


	/** Will Start running the Turn Logic.
	 *
	 * @param InitialTeam Is the team that will be the fist one to attack the opponent.
	 */
	virtual void RunTurnState(ERPGTeam InitialTeam , ERPGTeam& out_LastTeam);

	/** Tries to Branch between Teams Turn.
	 *
	 * @return  True if all the Players inside has made a movement this turn.
	 */
	virtual bool TryTurnBranch(ERPGTeam ActualTeam, ERPGTeam& LastTeam);

	/** Checks if all the Charactes from a Team have made a movement this round.
	 *
	 * @param Characters Couls be enemy team or ally team, it depends.Maybe the Player team got caught by surprise, so in that case the enemy team will attack first.
	 */
	virtual bool CheckAllCharactesMoved(TArray<ARPGCharacterBase*>Characters);


	/** --------------------------------------------------------------------------------------------------------- */

	
	/** Delegates */


	FOnPlayerFinishedTurn OnPlayerFinishedTurnDelegate;

	FOnTeamTurnFinished OnTeamTurnFinishedDelegate;

	FOnFinishBattle OnBattleFinishDelegate;




	/** --------------------------------------------------------------------------------------------------------- */

	protected:

	
	ERPGTeam LastBattleTeam;

	/** Delegates */

	UFUNCTION(BlueprintImplementableEvent)
    void OnBattleFinished();

	UFUNCTION(BlueprintImplementableEvent)
    void OnPlayerTurnEnded();

	UFUNCTION(BlueprintImplementableEvent)
    void OnTeamTurnEnded(ERPGTeam LastTeaminBattle);

	/** Delegate Functions Call Backs */

	void FinishBattle(EBattleMachineEndState EndState);

	void PlayerFinishedTurn();

	void TeamFinishedTurn(ERPGTeam Team);



	/** --------------------------------------------------------------------------------------------------------- */

	private:

	uint32 bHasBattleStarted : 1;



};




