// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RPGBattleMachine.generated.h"

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


	/** The core Functions */

	
	/** Function that will transport the Player to the BattleMap to Start the Battle. */
	void StartBattle();

	UFUNCTION(BlueprintCallable)
	void BattleMachine();	
};
