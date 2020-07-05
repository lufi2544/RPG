// Copyright Epic Games, Inc. All Rights Reserved.

#include "RPGStateMachineCommands.h"

#define LOCTEXT_NAMESPACE "FRPGStateMachineModule"

void FRPGStateMachineCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "RPGStateMachine", "Bring up RPGStateMachine window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
