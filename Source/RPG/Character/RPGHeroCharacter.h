// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGCharacterBase.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "RPGHeroCharacter.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API ARPGHeroCharacter : public ARPGCharacterBase
{
	GENERATED_BODY()

public:

	ARPGHeroCharacter();

	virtual void PossessedBy(AController* NewController) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	class USpringArmComponent* GetCameraBoom();

	class UCameraComponent* GetFollowCamera();
	
	UFUNCTION(BlueprintPure , Category= "RPG|HeroCharacter")
	FVector GetStartingCameraBoomLocation();

	UFUNCTION(BlueprintPure , Category = "RPG|HeroCharacter")
	float GetStartingSpringCameraBoomLenght();
	
protected:
	
	virtual void BeginPlay() override;

	virtual void OnRep_PlayerState() override;

	virtual void PostInitializeComponents() override;

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

	UPROPERTY(EditDefaultsOnly , EditAnywhere , Category = "RPG|Camera")
	float CameraBoomDistanceToCharacter;

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
	
};
