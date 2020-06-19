// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG/Public/RPGEngineSubsystem.h"

#include "AbilitySystemGlobals.h"

void URPGEngineSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UAbilitySystemGlobals::Get().InitGlobalData();
}
