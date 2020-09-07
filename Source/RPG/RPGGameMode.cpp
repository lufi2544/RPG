// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "RPGGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

ARPGGameMode::ARPGGameMode()
{


}

void ARPGGameMode::RPGGameModeStartBattle(ARPGPlayerController* PlayerController,ERPGMapTravelState MapTravelState)
{
    
    FString MapURL = "null";

   /** We have to implement a new array of connected players if we want to implement multiplayer in the future. So we can travel all the players from that array later. */

    /** In this occassion I will Manage the Battle Srtart In a way that the enemies will be Teleported to certain Places then we play the Camera animation
     *and then we Start the Battle (...)
     *
     */
    //It is obligatory to add the ?listen at the end of the MapURL so the server will listen for the player connections, if we do not add this, it won`t listen
    // for any calls to enter the level.
    /*
    if (MapTravelState == ERPGMapTravelState::Battle)
    {        
        MapURL = "/Game/Maps/BattleMap?listen";    
    }

    if (PlayerController && (MapURL != "null"))
    {
        
        bool bSuccess = false;
        UWorld* World = GetWorld();
        if (World)
        {
            bSuccess =  GetWorld()->ServerTravel(MapURL,true );

            PlayerController->ClientTravel(MapURL,ETravelType::TRAVEL_Absolute);
        }

        UE_LOG(LogTemp , Error , TEXT(" LOL %f"),bSuccess );

    }
    */

    
}
