// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGCharacterBase.h"
#include "RPGPlayerInventoryComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "RPG/RPG.h"

#include "RPGHeroCharacter.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API ARPGHeroCharacter : public ARPGCharacterBase
{
	GENERATED_BODY()

public:

	ARPGHeroCharacter(const class FObjectInitializer& ObjectInitializer);


	void GetLifetimeReplicatedProps( TArray< FLifetimeProperty > & OutLifetimeProps ) const override;
	
	virtual void PossessedBy(AController* NewController) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	class USpringArmComponent* GetCameraBoom();

	class UCameraComponent* GetFollowCamera();

	ERPGCharacterHeroType GetCharacterHeroType() const;
	
	ERPGAnimationMode GetCharacterAnimationMode() const;

	void SetCharacterHeroType( ERPGCharacterHeroType NewCharacterHeroType);
	
	void SetCharacterAnimationMode( ERPGAnimationMode NewCharacterAnimationMode);
	
	UFUNCTION(BlueprintPure , Category= "RPG|HeroCharacter")
	FVector GetStartingCameraBoomLocation();

	UFUNCTION(BlueprintPure , Category = "RPG|HeroCharacter")
	float GetStartingSpringCameraBoomArmLenght();

	UFUNCTION(BlueprintPure, Category = "RPG|HeroCharacter")
	ARPGPlayerInventoryComponent* GetPlayerInventoryComponent() const;
	
protected:
	
	virtual void BeginPlay() override;

	virtual void OnRep_PlayerState() override;

	// This is the Character Hero type, it may change over the game, so I decided to just add that on the Player State, because is something that can
	//be changed eventually.
	UPROPERTY(BlueprintReadWrite, EditAnywhere , Category= "RPG|HeroCharacter" , Replicated)
    ERPGCharacterHeroType CharacterHeroType = ERPGCharacterHeroType::NoWeapon;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere , Category= "RPG|HeroCharacter|Animation" , Replicated)
    ERPGAnimationMode CharacterAnimationMode = ERPGAnimationMode::NoWeapon;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RPG|Camera")
	float BaseTurnRate = 45.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RPG|Camera")
    float BaseLookUpRate = 45.0f;

	UPROPERTY(BlueprintReadOnly,VisibleAnywhere , Category = "RPG|Camera" )
	USpringArmComponent* CameraBoom;

	UPROPERTY(BlueprintReadOnly , VisibleAnywhere , Category = "RPG|Camera")
	UCameraComponent* FollowCamera;

	UPROPERTY(BlueprintReadOnly, Category = "RPG|Camera")
	float StartingCameraBoomArmLenght;

	UPROPERTY(BlueprintReadOnly, Category = "RRPG|Camera")
	FVector StartingCameraBoomLocation;

	
	

	//Mouse
	void LookUp(float fValue);

	//GamePad
	void LookUpRate(float fValue);

	//Mouse
	void Turn(float fValue);

	//GamePad
	void TurnRate(float fValue);

	//Mouse + GamePad
	void MoveForward(float fValue);

	//Mouse + GamePad
	void MoveRight(float fValue);

	bool ASCInputBound = false;

	// Called from both SetupPlayerInputComponent and OnRep_PlayerState because of a potential race condition where the PlayerController might
	// call ClientRestart which calls SetupPlayerInputComponent before the PlayerState is repped to the client so the PlayerState would be null in SetupPlayerInputComponent.
	// Conversely, the PlayerState might be repped before the PlayerController calls ClientRestart so the Actor's InputComponent would be null in OnRep_PlayerState.
	void BindASCInput();

	private:

	UPROPERTY(BlueprintReadOnly, Category = "RPG|Inventory" , Replicated ,meta= (AllowPrivateAccess = "true") )
    ARPGPlayerInventoryComponent* InventoryComponent;
	
};
