// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"

#include "Subsystems/EngineSubsystem.h"
#include "RPGEngineSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API URPGEngineSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

    
};
