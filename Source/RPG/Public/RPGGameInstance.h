// Ishak Hero.All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Character/RPGPlayerController.h"
#include "Engine/GameInstance.h"
#include "RPG/RPGGameMode.h"

#include "RPGGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API URPGGameInstance : public UGameInstance
{
	GENERATED_BODY()

	public:

	UFUNCTION(BlueprintCallable , Category = "RPG|GameInstance")
	virtual void TravelPlayer(ARPGPlayerController* PC , ERPGMapTravelState MapTravelState);
	
};
