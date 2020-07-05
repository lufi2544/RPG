// Copyright Epic Games, Inc. All Rights Reserved.

#include "RPGStateMachine.h"
#include "RPGStateMachineStyle.h"
#include "RPGStateMachineCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"

static const FName RPGStateMachineTabName("RPGStateMachine");

#define LOCTEXT_NAMESPACE "FRPGStateMachineModule"

void FRPGStateMachineModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FRPGStateMachineStyle::Initialize();
	FRPGStateMachineStyle::ReloadTextures();

	FRPGStateMachineCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FRPGStateMachineCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FRPGStateMachineModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FRPGStateMachineModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(RPGStateMachineTabName, FOnSpawnTab::CreateRaw(this, &FRPGStateMachineModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FRPGStateMachineTabTitle", "RPGStateMachine"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);

	UE_LOG(LogTemp,Error,TEXT("RPGStateMachine Running !!!"));
}

void FRPGStateMachineModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FRPGStateMachineStyle::Shutdown();

	FRPGStateMachineCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(RPGStateMachineTabName);
}

TSharedRef<SDockTab> FRPGStateMachineModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FRPGStateMachineModule::OnSpawnPluginTab")),
		FText::FromString(TEXT("RPGStateMachine.cpp"))
		);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(WidgetText)
			]
		];
}

void FRPGStateMachineModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->InvokeTab(RPGStateMachineTabName);
}

void FRPGStateMachineModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FRPGStateMachineCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("Settings");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FRPGStateMachineCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FRPGStateMachineModule, RPGStateMachine)