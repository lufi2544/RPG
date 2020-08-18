// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/RPGPlayerController.h"
#include "UObject/NoExportTypes.h"
#include "RPGBattleMachine.generated.h"


class ARPGEnemy;

UENUM(BlueprintType)
enum class EBattleMachineEndState : uint8
{


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
 * This is a Class that will manage all the Battle Logic, so if you want to add a new battle method, add that method here.This will manage all the checks during battle, etc
 */
UCLASS()
class RPGSTATEMACHINE_API URPGBattleMachine : public UObject
{
	GENERATED_BODY()

	public:

	// We will have to manage the Destruction of the State Machine later.
	
	URPGBattleMachine();

	/** The Main Player Controller controlled by the Player. */
	ARPGPlayerController* & MainPlayerController;

	/** Ally Charactes that has entered the Battle. */
	TArray<ARPGHeroCharacter*> AllyCharacters;

	/** Enemy Characters that has entered the Battle. */
	TArray<ARPGEnemy*> EnemyCharacters;


	/** The Core Battle State Machine Functions */

	
	/** Function that will transport the Player to the BattleMap to Start the Battle.
	 *
	 * @param Enemies The Enemies that have been Spawned on the Map.
	 * @param Allies The Ally Charactes who are on the map. 
	 */
	void StartBattle(TArray<ARPGEnemy*>&Enemies , TArray<ARPGHeroCharacter*>& Allies);

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
	 * @return  True if all the Players inside has moved.
	 */
	virtual bool TryTurnBranch(ERPGTeam ActualTeam, ERPGTeam& LastTeam);

	/** Checks if all the Charactes from a Team have made a movement this round.
	 *
	 * @param Characters Couls be enemy team or ally team, it depends.Maybe the Player team got caught by surprise, so in that case the enemy team will attack first.
	 */
	virtual bool CheckAllCharactesMoved(TArray<ARPGCharacterBase*>Characters);


	/** Delegates */


	FOnPlayerFinishedTurn OnPlayerFinishedTurnDelegate;

	FOnTeamTurnFinished OnTeamTurnFinishedDelegate;

	FOnFinishBattle OnBattleFinishDelegate;


	protected:

	ERPGTeam LastTeam;
	
	
	
};
