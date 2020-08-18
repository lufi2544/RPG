// Fill out your copyright notice in the Description page of Project Settings.
#include "RPGBattleMachine.h"
#include "RPGEnemy.h"
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

void URPGBattleMachine::RunBattleStateMachine(EBattleMachineEndState& EndState , ERPGTeam TeamToInitiate ,  ERPGTeam& out__LastTeam)
{
    //TODO Add the Current Logic for the State Machine.

    TryCheckBattleState(EndState);

    RunTurnState(TeamToInitiate,out__LastTeam);
}

void URPGBattleMachine::StopBattleStateMachine(EBattleMachineEndState &EndState)
{    
    //TODO Add the Escape Logic.

    OnBattleFinishDelegate.Broadcast(EndState);

    // Empty the Character Arrays and Reset variables.
    EnemyCharacters.Empty();
    AllyCharacters.Empty();
    MainPlayerController = nullptr;
    
}

void URPGBattleMachine::RunTurnState(ERPGTeam InitialTeam , ERPGTeam& out_LastTeam )
{
    //  All the Players has Finished Their turns ?
    if (TryTurnBranch(InitialTeam , out_LastTeam))
    {
        LastTeam = out_LastTeam;
        OnTeamTurnFinishedDelegate.Broadcast(LastTeam);
    }else
    {
        // Go to the next player.
        OnPlayerFinishedTurnDelegate.Broadcast();
    }
    
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

            /** All Enemies Dead */

            BattleMachineState = EBattleMachineState::Rejected;
            BattleMachineEndState = EBattleMachineEndState::OutOfEnemies;
            StopBattleStateMachine(BattleMachineEndState);
            
            return BattleMachineState;
        }
        
        //Player Turn Logic

        BattleMachineState = EBattleMachineState::Accepted;
        
     // State Machine got rejected by killing all enemies.   
    }else
    {
        /** All Allies Dead **/
        // If we run out of Allies means that we have lost the Battle and that we are out of allies;

        
        BattleMachineState = EBattleMachineState::Rejected;
        BattleMachineEndState = EBattleMachineEndState::OutOfAllies;
        StopBattleStateMachine(BattleMachineEndState);
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

bool URPGBattleMachine::TryTurnBranch(ERPGTeam ActualTeam,ERPGTeam& LastTeam)
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

    LastTeam = ActualTeam;

    if (CheckAllCharactesMoved(CharactersToCheck))
    {

        bSucces = true;
        LastTeam = ActualTeam;
        
    }
    
    /** If the Charactes have finished of moving in the turn, then we can change the turn to the other team. */
    return bSucces;
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
