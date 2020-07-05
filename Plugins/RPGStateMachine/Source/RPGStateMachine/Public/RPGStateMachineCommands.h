// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "RPGStateMachineStyle.h"

class FRPGStateMachineCommands : public TCommands<FRPGStateMachineCommands>
{
public:

	FRPGStateMachineCommands()
		: TCommands<FRPGStateMachineCommands>(TEXT("RPGStateMachine"), NSLOCTEXT("Contexts", "RPGStateMachine", "RPGStateMachine Plugin"), NAME_None, FRPGStateMachineStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};