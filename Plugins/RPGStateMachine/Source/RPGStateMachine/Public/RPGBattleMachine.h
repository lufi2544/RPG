// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGEnemy.h"
#include "Character/RPGPlayerController.h"
#include "UObject/NoExportTypes.h"
#include "RPGBattleMachine.generated.h"


UENUM(BlueprintType)
enum EBattleMachineEndState
{


	// There is no more enemies on the Map, all deffeted or they have may  escaped.
	OutOfEnemies,

	// there is no more allies on the Map, all defeted or they have may escaped
	OutOfAllies,

	None


};


UENUM(BlueprintType)
enum EBattleMachineState
{

	// We continue Branching
	Accepted,

	// The State got rejected by any means
	Rejected,

	Idle
	
};



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
	void RunBattleStateMachine();

	/** Stops the State Machine */
	void StopBattleStateMachine();

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


	/** Tries to Branch between Teams Turn.
	 *
	 * @return  True if all the Players inside has moved.
	 */
	virtual bool TryTurnBranch(ERPGTeam ActualTeam);

	virtual bool CheckAllCharactesMoved(TArray<ARPGCharacterBase*>Characters);


};
