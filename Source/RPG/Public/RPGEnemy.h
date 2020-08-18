// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/RPGCharacterBase.h"
#include "RPGEnemy.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API ARPGEnemy : public ARPGCharacterBase
{
	GENERATED_BODY()

	public:

	ARPGEnemy( const class FObjectInitializer& ObjectInitializer);



	protected:

		//Hard Pointer To the AbilitySystemComponent
		UPROPERTY()
		class URPGAbilitySystemComponent* HardRefAbilitySystemcomponent;

		//Hard Pointer to the AttributeSetBase
		UPROPERTY()
		class URPGAttributeSetBase* HardRefAttributeSetBase;

	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite , EditAnywhere , category = "RPG|UI")
	TSubclassOf<class URPGFloatingStatusBarWidget> UIFloatingStatusBarClass;

	UPROPERTY()
	class URPGFloatingStatusBarWidget* UIFloatingStatusBar;

	UPROPERTY(BlueprintReadOnly , VisibleAnywhere , Category = "RPG|UI")
	class UWidgetComponent* UIFloatingStatusBarComponent;

	FDelegateHandle HealthChangedDelegate;


	//Attribute changed callbacks
	virtual void Healthchanged(const FOnAttributeChangeData& Data);

	
	
};

