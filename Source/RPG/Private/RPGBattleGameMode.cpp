// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGBattleGameMode.h"

#include "RPGEnemy.h"

void ARPGBattleGameMode::StartBattle(TArray<ARPGEnemy*>Enemies , TArray<ARPGHeroCharacter*> Allies)
{

    if(Enemies.Num() < 1 || Allies.Num() < 1)
    {
        UE_LOG(RPGLog , Error , TEXT("The Array of enemies or allies that are trying to enter the Battle is 0"));
    }
    
    BattleMachine->StartBattle(Enemies , Allies);

    BattleMachine->OnBattleFinishDelegate.AddDynamic(this , &ARPGBattleGameMode::FinishBattle);
    BattleMachine->OnPlayerFinishedTurnDelegate.AddDynamic(this , &ARPGBattleGameMode::PlayerFinishedTurn);
    BattleMachine->OnTeamTurnFinishedDelegate.AddDynamic(this , &ARPGBattleGameMode::TeamFinishedTurn);

}

void ARPGBattleGameMode::RunBattle(EBattleMachineEndState& EndState, ERPGTeam TeamToInitiate, ERPGTeam& out_LastTeam)
{
    BattleMachine->RunBattleStateMachine(EndState , TeamToInitiate,out_LastTeam);
}

void ARPGBattleGameMode::FinishBattle(EBattleMachineEndState EndState)
{

    OnBattleFinished();
    
}

void ARPGBattleGameMode::PlayerFinishedTurn()
{
    OnPlayerTurnEnded();
}

void ARPGBattleGameMode::TeamFinishedTurn(ERPGTeam Team)
{
    OnTeamTurnEnded();
    
}
