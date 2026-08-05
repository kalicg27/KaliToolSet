// Copyright Epic Games, Inc. All Rights Reserved.

#include "KaliTools.h"
#include "ContentBrowserModule.h"

#define LOCTEXT_NAMESPACE "FKaliToolsModule"

void FKaliToolsModule::StartupModule()
{
	InitCBMenuExtention();
	
}
#pragma region ContentBrowserMenuExtention
void FKaliToolsModule::InitCBMenuExtention()
{	
	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));
	
	TArray<FContentBrowserMenuExtender_SelectedPaths>& ContentBrowserModuleMenuExtenders = 
	ContentBrowserModule.GetAllPathViewContextMenuExtenders(); 
	
	/*FContentBrowserMenuExtender_SelectedPaths CustomCBMenuDelegate;
	CustomCBMenuDelegate.BindRaw(this, &FKaliToolsModule::CustomCBMenuExtender);

	ContentBrowserModuleMenuExtenders.Add(CustomCBMenuDelegate);*/

	ContentBrowserModuleMenuExtenders.Add(FContentBrowserMenuExtender_SelectedPaths::CreateRaw(this, &FKaliToolsModule::CustomCBMenuExtender));

}

TSharedRef<FExtender> FKaliToolsModule::CustomCBMenuExtender(const TArray<FString>& SelectedPaths)
{
	TSharedRef<FExtender> MenuExtender(new FExtender());

	if (SelectedPaths.Num() > 0)
	{
		MenuExtender->AddMenuExtension(FName("Delete"),
			EExtensionHook::After,
			TSharedPtr<FUICommandList>(),
			FMenuExtensionDelegate::CreateRaw(this, &FKaliToolsModule::AddCBMenuEntry));

		MenuExtender->AddMenuExtension(FName("AddToFavorites"),
			EExtensionHook::After,
			TSharedPtr<FUICommandList>(),
			FMenuExtensionDelegate::CreateRaw(this, &FKaliToolsModule::AddCBMenuEntryOwn));
	}
	return MenuExtender;
}

void FKaliToolsModule::AddCBMenuEntry(FMenuBuilder& MenuBuilder)
{
	MenuBuilder.AddMenuEntry
	(
		FText::FromString(TEXT("MyOwnPluginTest")),
		FText::FromString(TEXT("Safely Create and use tools")),
		FSlateIcon(),
		FExecuteAction::CreateRaw(this, &FKaliToolsModule::OnMyOwnPluginButtonClicked)
	);
	
}
void FKaliToolsModule::AddCBMenuEntryOwn(FMenuBuilder& MenuBuilder)
{
	MenuBuilder.AddMenuEntry(
		FText::FromString(TEXT("ImplementetionTools")),
		FText::FromString(TEXT("Tools for Implementetion team")),
		FSlateIcon(),
		FExecuteAction::CreateRaw(this, &FKaliToolsModule::OnMyOwnPluginButtonClicked)
	);
}
void FKaliToolsModule::OnMyOwnPluginButtonClicked()
{
	
}

void FKaliToolsModule::OnImplementetionToolsButtonClicked()
{
}
#pragma endregion

void FKaliToolsModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FKaliToolsModule, KaliTools)