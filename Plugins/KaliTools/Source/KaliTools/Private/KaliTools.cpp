// Copyright Epic Games, Inc. All Rights Reserved.

#include "KaliTools.h"
#include "ContentBrowserModule.h"
#include "DebugHeader.h"
#include "EditorAssetLibrary.h"
#include "ObjectTools.h"
#include "AssetToolsModule.h"
#include "SlateWidgets/ImplToolsWidget.h"
#include "HAL/IConsoleManager.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "WorkspaceMenuStructure.h"
#include "WorkspaceMenuStructureModule.h"

#include "Engine/Blueprint.h"
#include "Engine/CurveTable.h"
#include "Engine/DataTable.h"
#include "Editor/UnrealEd/Public/Kismet2/KismetEditorUtilities.h"
#include "SourceControlHelpers.h"
#include "FileHelpers.h"
#include "KaliToolsPresets.h"

#define LOCTEXT_NAMESPACE "FKaliToolsModule"

UCurveTable* FKaliToolsModule::GetDefaultCurveTable()
{
	return Cast<UCurveTable>(FKaliToolsPresets::DefaultCurveTablePath.TryLoad());
}

UDataTable* FKaliToolsModule::GetDefaultDataTable()
{
	return Cast<UDataTable>(FKaliToolsPresets::DefaultDataTablePath.TryLoad());
}
void FKaliToolsModule::StartupModule()
{
	InitCBMenuExtention();
	RegistesterImplementetionToolsTab();
	
	
}
static const FName ImplementationToolsTabId("ImplementetionTools");
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

		FolderPathsSelected = SelectedPaths;
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
		FExecuteAction::CreateRaw(this, &FKaliToolsModule::OnImplementetionToolsButtonClicked)
	);
}
void FKaliToolsModule::OnMyOwnPluginButtonClicked()
{
	//nothin just test
	//UE_LOG(LogTemp, Warning, TEXT("Should be working "));
}

void FKaliToolsModule::OnImplementetionToolsButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(FName(ImplementationToolsTabId));
	
}
#pragma endregion

#pragma region CustomEditorTab

void FKaliToolsModule::RegistesterImplementetionToolsTab()
{
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(FName(ImplementationToolsTabId),
		FOnSpawnTab::CreateRaw(this, &FKaliToolsModule::OnSpawnImplementetionToolsTab))
		.SetDisplayName(FText::FromString(TEXT("Implementetion Tools")));
}

TSharedRef<SDockTab> FKaliToolsModule::OnSpawnImplementetionToolsTab(const FSpawnTabArgs &SpawnTabArgs)
{
	return
	SNew(SDockTab).TabRole(ETabRole::NomadTab)
	[
		SNew(SImplToolsTab)
		.AssetsDataToStore(GetAllSelectedAssets())
	];
	 
}


//TSharedRef<SDockTab> FKaliToolsModule::ImplBlueprintSetUpTab(const FSpawnTabArgs& SpawnTabArgs)
//{
//	return TSharedRef<SDockTab>();
//}

TArray<TSharedPtr<FAssetData>> FKaliToolsModule::GetAllSelectedAssets()
{
	TArray<TSharedPtr<FAssetData>> AvailableAssetsData;

	if (FolderPathsSelected.Num() == 0)
	{
		return AvailableAssetsData;
	}

	TArray<FString> AssetsPathNames = UEditorAssetLibrary::ListAssets(FolderPathsSelected[0], true );

	for (const FString& AssetPathName : AssetsPathNames)
	{
		if (AssetPathName.Contains(TEXT("Developers")) ||
			AssetPathName.Contains(TEXT("Collections")) ||
			AssetPathName.Contains(TEXT("__ExternalActors__")) ||
			AssetPathName.Contains(TEXT("__ExternalObjects__")))
		{
			continue;
		}

		if (!UEditorAssetLibrary::DoesAssetExist(AssetPathName))
		{
			continue;
		}

		const FAssetData Data = UEditorAssetLibrary::FindAssetData(AssetPathName);
		AvailableAssetsData.Add(MakeShared<FAssetData>(Data));
	}

	return AvailableAssetsData;
}
#pragma endregion

#pragma region CustomToolsFunction

void FKaliToolsModule::SetForcedLOD(int32 LODLevel)
{
	static IConsoleVariable* CVarNanite = IConsoleManager::Get().FindConsoleVariable(TEXT("r.nanite"));
	if (CVarNanite)
	{
		CVarNanite->Set(0, ECVF_SetByCode);
	}

	static IConsoleVariable* CVarForceLOD = IConsoleManager::Get().FindConsoleVariable(TEXT("r.ForceLOD"));
	if (CVarForceLOD)
	{
		CVarForceLOD->Set(LODLevel, ECVF_SetByCode);
	}
}

bool FKaliToolsModule::DeleteSingleAssetForList(const FAssetData& AssetDataToDelete)
{
	TArray<FAssetData> AssetDatatForDeletion;
	AssetDatatForDeletion.Add(AssetDataToDelete);

	if (ObjectTools::DeleteAssets(AssetDatatForDeletion) > 0)
	{
		return true;
	}
	return false;
}

UCurveTable* FKaliToolsModule::FetchCurveTableFromBlueprint(const FAssetData& AssetData)
{
	UBlueprint* BP = Cast<UBlueprint>(AssetData.GetAsset());
	if (!BP || !BP->GeneratedClass)
	{
		return GetDefaultCurveTable();
	}

	UObject* CDO = BP->GeneratedClass->GetDefaultObject();

	// Attempt reflection search for BuildingResourceAmountOverride
	FStructProperty* OverrideProp = FindFProperty<FStructProperty>(CDO->GetClass(), TEXT("BuildingResourceAmountOverride"));
	if (OverrideProp && OverrideProp->Struct == FCurveTableRowHandle::StaticStruct())
	{
		FCurveTableRowHandle* HandlePtr = OverrideProp->ContainerPtrToValuePtr<FCurveTableRowHandle>(CDO);
		if (HandlePtr && HandlePtr->CurveTable)
		{
			return HandlePtr->CurveTable;
		}
	}

	// Fallback to default specified in KaliToolsPresets.h
	return GetDefaultCurveTable();
}

TArray<FName> FKaliToolsModule::GetRowNamesFromCurveTable(UCurveTable* TargetCurveTable)
{
	TArray<FName> RowNames;
	if (TargetCurveTable)
	{
		for (const auto& Pair : TargetCurveTable->GetRowMap())
		{
			RowNames.Add(Pair.Key);
		}
	}
	return RowNames;
}

bool FKaliToolsModule::CopyParametersFromBlueprint(const FAssetData& AssetData, FKaliBlueprintSetupParams& OutParams)
{
	UBlueprint* BP = Cast<UBlueprint>(AssetData.GetAsset());
	if (!BP || !BP->GeneratedClass) return false;

	UObject* CDO = BP->GeneratedClass->GetDefaultObject();
	if (!CDO) return false;

	// 1. Fetch Dynamic Curve Table or fallback
	UCurveTable* ActiveCurveTable = FetchCurveTableFromBlueprint(AssetData);
	OutParams.BuildingResourceAmountOverride.CurveTable = ActiveCurveTable;

	// 2. Extract internal property values using reflection
	if (FStructProperty* ResProp = FindFProperty<FStructProperty>(CDO->GetClass(), TEXT("BuildingResourceAmountOverride")))
	{
		FCurveTableRowHandle* Handle = ResProp->ContainerPtrToValuePtr<FCurveTableRowHandle>(CDO);
		if (Handle)
		{
			OutParams.BuildingResourceAmountOverrideRowName = Handle->RowName;
		}
	}

	// 3. Extract Booleans and Tags
	if (FBoolProperty* BoolProp = FindFProperty<FBoolProperty>(CDO->GetClass(), TEXT("bCreatePhys")))
	{
		OutParams.bCreatePhys = BoolProp->GetPropertyValue_InContainer(CDO);
	}

	return true;
}

bool FKaliToolsModule::ApplyParametersToBlueprint(const FAssetData& AssetData, const FKaliBlueprintSetupParams& InParams)
{
	UBlueprint* BP = Cast<UBlueprint>(AssetData.GetAsset());
	if (!BP || !BP->GeneratedClass) return false;

	CheckoutAndSaveAsset(BP);

	UObject* CDO = BP->GeneratedClass->GetDefaultObject();

	// Write internal parameters via reflection
	if (FStructProperty* ResProp = FindFProperty<FStructProperty>(CDO->GetClass(), TEXT("BuildingResourceAmountOverride")))
	{
		FCurveTableRowHandle* Handle = ResProp->ContainerPtrToValuePtr<FCurveTableRowHandle>(CDO);
		if (Handle)
		{
			Handle->CurveTable = InParams.BuildingResourceAmountOverride.CurveTable;
			Handle->RowName = InParams.BuildingResourceAmountOverrideRowName;
		}
	}

	BP->Modify();
	FKismetEditorUtilities::CompileBlueprint(BP);

	UPackage* Package = BP->GetOutermost();
	FEditorFileUtils::PromptForCheckoutAndSave({ Package }, false, false);

	return true;
}

bool FKaliToolsModule::ValidateBlueprintAsset(const FAssetData& AssetData, TArray<FText>& OutValidationErrors)
{
	if (!AssetData.IsValid())
	{
		OutValidationErrors.Add(FText::FromString("Selected Asset is Invalid."));
		return false;
	}
	return true;
}

bool FKaliToolsModule::CheckoutAndSaveAsset(UObject* AssetToSave)
{
	if (!AssetToSave) return false;
	FString PackageName = AssetToSave->GetOutermost()->GetName();
	if (SourceControlHelpers::IsAvailable())
	{
		return SourceControlHelpers::CheckOutFile(PackageName);
	}
	return true;
}

#pragma endregion

void FKaliToolsModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FKaliToolsModule, KaliTools)