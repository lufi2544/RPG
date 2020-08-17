// Fill out your copyright notice in the Description page of Project Settings.
#include "RPGBattleMachine.h"

#include "RPG/Public/Character/RPGHeroCharacter.h"


URPGBattleMachine::URPGBattleMachine()
{
}

void URPGBattleMachine::StartBattle(TArray<ARPGEnemy*>& Enemies, TArray<ARPGHeroCharacter*>& Allies)
{

    // We set the references once we enter the battle map.
    AllyCharacters = Allies;
    EnemyCharacters = Enemies;

    //TODO Add the UI.

    
    
}

void URPGBattleMachine::RunBattleStateMachine()
{
    //TODO Add the Current Logic for the State Machine.
}

void URPGBattleMachine::StopBattleStateMachine()
{
//TODO Add the CallBack to the GameMode.
    
}



EBattleMachineState URPGBattleMachine::TryCheckBattleState(EBattleMachineEndState& BattleMachineEndState)
{
    EBattleMachineState BattleMachineState = EBattleMachineState::Idle;
    
    // 1 -> Check the Player State, we have to make sure that the Ally Team are still Alive.
    // 2 -> If Theay are still alive we will branch again.That Means that we will enter the next turn.
         // If they are not, the State will be rejected.

    // All Allies defeated ?
    if (CheckPlayerStates())
    {
        // All enemies defeated ?
        if (!CheckEnemiesState())
        {


            BattleMachineState = EBattleMachineState::Rejected;
            BattleMachineEndState = EBattleMachineEndState::OutOfEnemies;
            
            return BattleMachineState;
        }
        
        //Player Turn Logic

        BattleMachineState = EBattleMachineState::Accepted;
        
     // State Machine got rejected by killing all enemies.   
    }else
    {
        // If we run out of Allies means that we have lost the Battle and that we are out of allies;
        
        BattleMachineState = EBattleMachineState::Rejected;
        BattleMachineEndState = EBattleMachineEndState::OutOfAllies;
    }
  
  return BattleMachineState;  
}

bool URPGBattleMachine::CheckPlayerStates()
{
    bool bSuccess = true;
    int32 CharacterIndex = AllyCharacters.Num();
    int32 CharactesDeadIndexCounter = 0;

    // We check if there is more than 1 Player in the Ally Array.
    if (CharacterIndex > 0)
    {
        // We Iterate All the Players and store all the dead Players, if it is equal to the Amout of Players, the State gets rejected and we return false.
        for(ARPGHeroCharacter* Character : AllyCharacters)
        {
            ARPGPlayerState* PS = Character->GetRPGPlayerState();

            if (PS)
            {
                if (!PS->IsAlive())
                {
                    CharactesDeadIndexCounter++;
                }
            }   
        }
    }

    if (CharacterIndex == CharactesDeadIndexCounter)
    {
        bSuccess = false;
    }
  
    return bSuccess;
}

bool URPGBattleMachine::CheckEnemiesState()
{
    bool bSuccess = true;
    int32 EnemyIndex = EnemyCharacters.Num();
    int32 EnemiesDeadIndexCounter = 0;

    if (EnemyIndex > 0)
    {
        for(ARPGEnemy* Character : EnemyCharacters)
        {
            ARPGPlayerState* PS = Character->GetRPGPlayerState();

            if (PS)
            {
                if (!PS->IsAlive())
                {
                    EnemiesDeadIndexCounter++;
                }
            }   
        } 
    }

    if (EnemiesDeadIndexCounter == EnemyIndex)
    {
        bSuccess = false;
    }

    return bSuccess;
}

bool URPGBattleMachine::TryTurnBranch(ERPGTeam ActualTeam)
{

    bool bSucces = false;
    int32 CharactersIdx;
    int32 CharacterMovedIdxCounter = 0;
    TArray<ARPGCharacterBase*>CharactersToCheck;


    /** We Iterate the Team depending on the turn that we are on and assign the Characters to check their state. */
    if (ActualTeam == ERPGTeam::Ally)
    {
         CharactersIdx = AllyCharacters.Num();
         for (int32 Idx = 0 ; Idx < AllyCharacters.Num(); Idx++)
         {
             CharactersToCheck.Add(Cast<ARPGCharacterBase>(AllyCharacters[Idx]));
         }
        
    }else if (ActualTeam == ERPGTeam::Enemy)
    {
        CharactersIdx = EnemyCharacters.Num();

        for (int32 Idx ; Idx < EnemyCharacters.Num(); Idx++)
        {
            CharactersToCheck.Add(Cast<ARPGCharacterBase>(EnemyCharacters[Idx]));
        }
        
    }

    /** If the Charactes have finished of moving in the turn, then we can change the turn to the other team. */
    return CheckAllCharactesMoved(CharactersToCheck);
}

bool URPGBattleMachine::CheckAllCharactesMoved(TArray<ARPGCharacterBase*> Characters)
{
    bool bSuccess = false;
    
    if (Characters.Num() > 0)
    {
        bool AllHasMoved = false;
        int32 PlayerMovedIdxCounter = 0;
        
        for (ARPGCharacterBase* Character : Characters)
        {
            ARPGPlayerState* PS = Character->GetRPGPlayerState();

            if (PS)
            {
                if (PS->GetPlayerTurnState() == true)
                {
                    PlayerMovedIdxCounter++;
                }
            }
        }

        if (PlayerMovedIdxCounter == Characters.Num())
        {
            bSuccess = true;
        }
    }
    
    return bSuccess;
}
