// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGCharacterBase.h"
#include "GameFramework/PlayerController.h"
#include "UI/RPGDamageTextWidgetComponent.h"
#include "RPG/Public/UI/RPGHUDWidget.h"
#include "RPGPlayerController.generated.h"


class URPGWidget;

/**
 * 
 */
UCLASS()
class RPG_API ARPGPlayerController : public APlayerController
{
	GENERATED_BODY()

	public:

	void CreateHUD();

	UPROPERTY(EditAnywhere , Category = "RPG|UI")
	TSubclassOf<class URPGDamageTextWidgetComponent> DamageNumberClass;

	class URPGHUDWidget* GetHUD();

	UFUNCTION(Server , Reliable,WithValidation)
	void ShowDamageNumber(float DamageAmount , ARPGCharacterBase* TargetCharacter);

	ARPGPlayerState* GetRPGPlayerStateFromController();


	protected:

	UPROPERTY(BlueprintReadWrite , EditAnywhere,Category= "RPG|UI")
	TSubclassOf<class URPGHUDWidget> UIHUDWidgetClass;

	UPROPERTY(BlueprintReadWrite, category = "RPG|UI")
	 class URPGHUDWidget* UIHUDWidget;

	
	// Server Only
	void OnPossess(APawn* InPawn) override;


	//For Clients
	void OnRep_PlayerState() override;


	
};
