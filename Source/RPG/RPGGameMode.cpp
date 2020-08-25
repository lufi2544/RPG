// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "RPGGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

ARPGGameMode::ARPGGameMode()
{


}

void ARPGGameMode::TravelPlayerToLevel(ARPGPlayerController* PlayerController,ERPGMapTravelState MapTravelState)
{
    FString MapURL = "null";
    
    if (MapTravelState == ERPGMapTravelState::Battle)
    {
        MapURL = "/Game/Maps/BattleMap.BattleMap";    
    }

    if (PlayerController && (MapURL != "null"))
    {
        bool bSuccess = false;
        UWorld* World = GetWorld();
        if (World)
        {
            bSuccess =  GetWorld()->ServerTravel(MapURL,true , true);
        }

        UE_LOG(LogTemp , Error , TEXT("%f"),bSuccess );

    }
    
}
