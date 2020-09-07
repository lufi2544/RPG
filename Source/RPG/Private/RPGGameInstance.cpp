// Ishak Hero.All Rights Reserved.


#include "RPGGameInstance.h"

#include "RPGBattleGameMode.h"

void URPGGameInstance::TravelPlayer(ARPGPlayerController* PC, ERPGMapTravelState MapTravelState)
{

    UWorld* World = GetWorld();
    ARPGGameMode* GM = nullptr;
    
    if (World)
    {
       GM =  Cast<ARPGGameMode>( World->GetAuthGameMode() );
    }


    if (GM)
    {
        GM->RPGGameModeStartBattle(PC,MapTravelState);
    }

    
}
