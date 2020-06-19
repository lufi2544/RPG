// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "RPGAbilitySystemComponent.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FRecievedDamageDelegate, URPGAbilitySystemComponent*, SourceASC, float,InitialDamage,float, FinalDamage);

/**
 * 
 */
UCLASS()
class RPG_API URPGAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	bool InitialAbilitiesInitialized = false;
	bool InitialEffectsInitializeded = false;

	FRecievedDamageDelegate ReceivedDamaged;

	virtual void ReceiveDamage(URPGAbilitySystemComponent* SourceASC, float InitialDamage,float FinalDamage);
	
};
