// Fill out your copyright notice in the Description page of Project Settings.
#include "RPGBattleMachine.h"


URPGBattleMachine::URPGBattleMachine()
{
}

void URPGBattleMachine::StartBattle()
{

    // Here is where all the Initial Events of the Battle Start will take place.
    
   
}

void URPGBattleMachine::StartBattleLogic()
{
}

void URPGBattleMachine::StopBattleLogic()
{

    
}

EBattleMachineState URPGBattleMachine::TryBattleBranch(EBattleMachineEndState& BattleMachineEndState)
{
    EBattleMachineState BattleMachineState = EBattleMachineState::Accepted;
    
    // 1 -> Check the Player State, we have to make sure that the Ally Team are still Alive.
    // 2 -> If Theay are still alive we will branch again.That Means that we will enter the next turn.

    if (CheckPlayerStates())
    {

        //TODO Add the Enemy Check.
        
        //Player Turn Logic

        BattleMachineState = EBattleMachineState::Accepted;
        
     // State Machine got rejected by killing all enemies.   
    }else
    {
        // If we run out of Allies means that we have lost the Battle and that we are out of allies;
        
        BattleMachineState = EBattleMachineState::Rejected;
        BattleMachineEndState = EBattleMachineEndState::PlayerLost;
    }
  
  return BattleMachineState;  
}

bool URPGBattleMachine::CheckPlayerStates()
{
    bool bSuccess = true;
    int32 PlayerIndex = Players.Num();
    int32 PlayerDeadIndexCounter = 0;

    // We Iterate All the Players and store all the dead Players, if it is equal to the Amout of Players, the State gets rejected and we return false.
    for(ARPGPlayerController* PC : Players)
    {
        ARPGPlayerState* PS = PC->GetRPGPlayerStateFromController();

        if (PS)
        {
            if (!PS->IsAlive())
            {
                PlayerDeadIndexCounter++;
            }
        }
        
    }

    if (PlayerIndex == PlayerDeadIndexCounter)
    {
        bSuccess = false;
    }

    
    return bSuccess;
}
