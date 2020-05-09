// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGEquipableItem.h"
#include "GameFramework/Actor.h"

#include "RPGShield.generated.h"

UCLASS()
class RPG_API ARPGShield : public ARPGEquipableItem
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARPGShield();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame


};
