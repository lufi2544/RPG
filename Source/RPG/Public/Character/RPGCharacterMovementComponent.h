// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RPGCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API URPGCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

	class FRPGSavedMode : public FSavedMove_Character
	{

	
	public:

		typedef FSavedMove_Character Super;
		///Cleans all the stored variables
		virtual void Clear() override;

		///Store Input Commands into the Compresed Flags
		virtual uint8 GetCompressedFlags() const override;

		///Virtual function that is used to checked if two moves can be combined.
		///Basically you just check to make sure that the saved variables are the same.
		virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* Character, float MaxDelta) const override;

		/// Sets up the move before sending it to the server. 
		virtual void SetMoveFor(ACharacter* Character, float InDeltaTime, FVector const& NewAccel, class FNetworkPredictionData_Client_Character & ClientData) override;
		
		///Sets variables on character movement component before making a predictive correction.
		virtual void PrepMoveFor(class ACharacter* Character)override;


		uint8 SavedRequestToStartSprinting : 1;
		uint8 SavedRequestToStartRolling : 1;
		uint8 SavedRequestToStartBlocking : 1;
		uint8 SavedRequestToStartBattleMode : 1;
	};

	class FRPGNetworkPredictionData_Client : public FNetworkPredictionData_Client_Character
	{
	public:
		FRPGNetworkPredictionData_Client(const UCharacterMovementComponent& ClientMovement);
		
		typedef FNetworkPredictionData_Client_Character Super;

		virtual FSavedMovePtr AllocateNewMove() override;
	};
public:
	URPGCharacterMovementComponent();

	UPROPERTY(EditAnywhere , BlueprintReadWrite, Category = "Sprint")
	float SprintMultiplier;

	
	uint8 RequestToStartSprinting :1;

	uint8 RequestToStartRolling : 1;

	uint8 RequestToStartBlocking : 1;

	uint8 RequestToStartBattleMode : 1;
	

	virtual float GetMaxSpeed() const override;
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;
	virtual class FNetworkPredictionData_Client* GetPredictionData_Client() const override;

	///Sprint
	UFUNCTION(BlueprintCallable , Category = "Sprint")
	void StartSprinting();
	UFUNCTION(BlueprintCallable , Category= "Sprint")
	void StopSprinting();

	///Rolling
	UFUNCTION(BlueprintCallable , Category = "Roll")
	void StartRolling();
	UFUNCTION(BlueprintCallable , Category = "Roll")
    void StopRolling();

	///Blocking
	UFUNCTION(BlueprintCallable , Category = "Blocking")
	void StartBlocking();
	UFUNCTION(BlueprintCallable , Category = "Blocking")
	void StopBlocking();

	///BattleMode
	UFUNCTION(BlueprintCallable , Category = "BattleMode")
    void StartBattleMode();
	UFUNCTION(BlueprintCallable , Category = "BattleMode")
    void StopBattleMode();

	
	
};
