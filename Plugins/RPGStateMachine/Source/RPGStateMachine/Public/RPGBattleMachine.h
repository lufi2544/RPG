// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/RPGPlayerController.h"
#include "UObject/NoExportTypes.h"
#include "RPGBattleMachine.generated.h"


UENUM(BlueprintType)
enum EBattleMachineEndState
{


	PlayerWon,

	PlayerLost,

	None


};


UENUM(BlueprintType)
enum EBattleMachineState
{

	// We continue Branching<
	Accepted,
	
	// The Enemies Amount is 0
	OutOfEnemies,

	// The Allies amount is 0
	OutOfAllies,

	// The State got rejected by any means
	Rejected
	
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

	/** The Amount of Players who have entered the Battle. */
	TArray<ARPGPlayerController*>&Players;


	/** The Core Battle State Machine Functions */

	
	/** Function that will transport the Player to the BattleMap to Start the Battle. */
	void StartBattle();

	void StartBattleLogic();

	EBattleMachineState TryBattleBranch(EBattleMachineEndState& BattleMachineEndState);

	bool CheckPlayerStates();

	void StopBattleLogic();


};
