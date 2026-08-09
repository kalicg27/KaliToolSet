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
#include "ImplPresetPluginData.h"

#include "Engine/Blueprint.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "FileHelpers.h"
#include "Editor.h"

#include "Engine/DataTable.h"
#include "Engine/CurveTable.h"

// Correct Viewport & Level Editor Includes
#include "LevelEditor.h"
#include "SLevelViewport.h"
#include "ILevelEditor.h"

// World Partition Includes
#include "WorldPartition/WorldPartitionEditorSubsystem.h"



#define LOCTEXT_NAMESPACE "FKaliToolsModule"

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

UCurveTable* FKaliToolsModule::GetDefaultCurveTable()
{
	return Cast<UCurveTable>(FImplPresetPluginData::DefaultCurveTablePath.TryLoad());
}

UDataTable* FKaliToolsModule::GetDefaultDataTable()
{
	return Cast<UDataTable>(FImplPresetPluginData::DefaultDataTablePath.TryLoad());
}

UCurveTable* FKaliToolsModule::FetchCurveTableFromBlueprint(const FAssetData& AssetData)
{
	UBlueprint* BP = Cast<UBlueprint>(AssetData.GetAsset());
	if (!BP || !BP->GeneratedClass)
	{
		return GetDefaultCurveTable();
	}

	UObject* CDO = BP->GeneratedClass->GetDefaultObject();

	FStructProperty* OverrideProp = FindFProperty<FStructProperty>(CDO->GetClass(), TEXT("BuildingResourceAmountOverride"));
	if (OverrideProp && OverrideProp->Struct == FCurveTableRowHandle::StaticStruct())
	{
		FCurveTableRowHandle* HandlePtr = OverrideProp->ContainerPtrToValuePtr<FCurveTableRowHandle>(CDO);
		if (HandlePtr && HandlePtr->CurveTable)
		{
			return const_cast<UCurveTable*>(HandlePtr->CurveTable.Get());
		}
	}

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

bool FKaliToolsModule::FetchBlueprintProperties(UBlueprint* TargetBP, const TArray<FName>& PropertyNames, TMap<FName, FString>& OutValues)
{
	OutValues.Empty();

	if (!TargetBP || !TargetBP->GeneratedClass) return false;

	UObject* CDO = TargetBP->GeneratedClass->GetDefaultObject();
	if (!CDO) return false;

	for (const FName& PropName : PropertyNames)
	{
		FProperty* Property = FindFProperty<FProperty>(TargetBP->GeneratedClass, PropName);
		if (Property)
		{
			const void* ValuePtr = Property->ContainerPtrToValuePtr<void>(CDO);
			FString ExportedText;

			// Handle Enums natively
			if (const FEnumProperty* EnumProp = CastField<FEnumProperty>(Property))
			{
				UEnum* EnumDef = EnumProp->GetEnum();
				int64 RawValue = EnumProp->GetUnderlyingProperty()->GetSignedIntPropertyValue(ValuePtr);
				ExportedText = EnumDef->GetNameStringByValue(RawValue);
			}
			else if (const FByteProperty* ByteProp = CastField<FByteProperty>(Property))
			{
				if (ByteProp->IsEnum())
				{
					// 👈 Use 'ByteProp->Enum' instead of 'ByteProp->GetEnum()'
					ExportedText = ByteProp->Enum->GetNameStringByValue(ByteProp->GetPropertyValue(ValuePtr));
				}
				else
				{
					Property->ExportTextItem_Direct(ExportedText, ValuePtr, nullptr, CDO, PPF_None);
				}
			}
			else
			{
				Property->ExportTextItem_Direct(ExportedText, ValuePtr, nullptr, CDO, PPF_None);
			}

			OutValues.Add(PropName, ExportedText);
		}
	}

	return true;
}

bool FKaliToolsModule::ApplyBlueprintProperties(UBlueprint* TargetBP, const TMap<FName, FString>& PropertyValuesToApply)
{
	if (!TargetBP || !TargetBP->GeneratedClass)
	{
		DebugHeader::ShowMsgDialog(
			EAppMsgType::Ok,
			TEXT("Failed to apply properties: Selected Blueprint is invalid or uncompiled."),
			true
		);
		return false;
	}

	UObject* CDO = TargetBP->GeneratedClass->GetDefaultObject();
	UPackage* Package = TargetBP->GetOutermost();

	if (!CDO || !Package)
	{	
		DebugHeader::ShowMsgDialog(
			EAppMsgType::Ok,
			TEXT("Failed to retrieve CDO or Package for the selected Blueprint."),
			true
		);
		return false;
	}

	// 1. Transaction Support (Undo/Redo)
	GEditor->BeginTransaction(FText::FromString("Modify Blueprint CDO Values via KaliTools"));

	CDO->Modify();
	TargetBP->Modify();

	bool bPropertyChanged = false;

	for (const TPair<FName, FString>& Pair : PropertyValuesToApply)
	{
		const FName& PropName = Pair.Key;
		const FString& NewTextValue = Pair.Value;

		FProperty* Property = FindFProperty<FProperty>(TargetBP->GeneratedClass, PropName);
		if (Property)
		{
			void* ValuePtr = Property->ContainerPtrToValuePtr<void>(CDO);

			// Import text string into CDO memory location
			Property->ImportText_Direct(*NewTextValue, ValuePtr, CDO, PPF_None);

			// Notify reflection system
			FPropertyChangedEvent PropertyChangeEvent(Property, EPropertyChangeType::ValueSet);
			CDO->PostEditChangeProperty(PropertyChangeEvent);

			bPropertyChanged = true;
		}
	}

	// 2. Recompile Blueprint
	if (bPropertyChanged)
	{
		FKismetEditorUtilities::CompileBlueprint(TargetBP);
	}

	GEditor->EndTransaction();

	// 3. Mark Package Dirty & Automatic Checkout/Save
	if (bPropertyChanged)
	{
		Package->MarkPackageDirty();

		TArray<UPackage*> PackagesToSave;
		PackagesToSave.Add(Package);

		FEditorFileUtils::EPromptReturnCode SaveResult = FEditorFileUtils::PromptForCheckoutAndSave(PackagesToSave, false, false);

		if (SaveResult == FEditorFileUtils::EPromptReturnCode::PR_Success)
		{
			UE_LOG(LogTemp, Log, TEXT("[KaliTools] Successfully saved Blueprint asset: %s"), *TargetBP->GetName());

			// Success Toast
			FString NotifyMsg = FString::Printf(TEXT("Successfully updated %s"), *TargetBP->GetName());
			DebugHeader::ShowNotifyInfo(NotifyMsg);

			return true;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("[KaliTools] Save/Checkout failed for: %s"), *TargetBP->GetName());

			
			FString FailMsg = FString::Printf(TEXT("Failed to save asset or source control checkout failed for:\n%s"), *TargetBP->GetName());
			DebugHeader::ShowMsgDialog(EAppMsgType::Ok, FailMsg, true);
		}
	}
	else
	{
		
		DebugHeader::ShowMsgDialog(
			EAppMsgType::Ok,
			FString::Printf(TEXT("No matching editable properties found to update on '%s'."), *TargetBP->GetName()),
			true
		);
	}

	return bPropertyChanged;
}

FKaliValidationResult FKaliToolsModule::ValidateBlueprintSetup(UBlueprint* TargetBP)
{
	FKaliValidationResult Result;

	// 1. Basic Blueprint Check
	if (!TargetBP)
	{
		Result.bIsValid = false;
		Result.ValidationErrors.Add(TEXT("Target Blueprint is null."));
		return Result;
	}

	if (!TargetBP->GeneratedClass)
	{
		Result.bIsValid = false;
		Result.ValidationErrors.Add(TEXT("Blueprint is not compiled. Please compile the Blueprint first."));
		return Result;
	}

	UObject* CDO = TargetBP->GeneratedClass->GetDefaultObject();
	if (!CDO)
	{
		Result.bIsValid = false;
		Result.ValidationErrors.Add(TEXT("Failed to retrieve CDO (Class Default Object)."));
		return Result;
	}

	// 2. Validate Specific Required Properties (DataTables, Curves, etc.)
	bool bFoundDataTable = false;
	bool bFoundCurveTable = false;

	for (TFieldIterator<FProperty> PropIt(TargetBP->GeneratedClass); PropIt; ++PropIt)
	{
		FProperty* Property = *PropIt;

		// Check for Object Reference Properties (FObjectProperty / FWeakObjectProperty)
		if (FObjectProperty* ObjProp = CastField<FObjectProperty>(Property))
		{
			UClass* PropertyClass = ObjProp->PropertyClass;

			// Check for Data Table Property
			if (PropertyClass->IsChildOf(UDataTable::StaticClass()))
			{
				bFoundDataTable = true;
				UObject* AssignedValue = ObjProp->GetObjectPropertyValue(ObjProp->ContainerPtrToValuePtr<void>(CDO));
				if (!AssignedValue)
				{
					Result.ValidationWarnings.Add(FString::Printf(TEXT("Property '%s' (DataTable) is unassigned!"), *Property->GetName()));
				}
			}
			// Check for Curve Table Property
			else if (PropertyClass->IsChildOf(UCurveTable::StaticClass()))
			{
				bFoundCurveTable = true;
				UObject* AssignedValue = ObjProp->GetObjectPropertyValue(ObjProp->ContainerPtrToValuePtr<void>(CDO));
				if (!AssignedValue)
				{
					Result.ValidationWarnings.Add(FString::Printf(TEXT("Property '%s' (CurveTable) is unassigned!"), *Property->GetName()));
				}
			}
		}
	}

	// 3. Strict Rules (Add errors if crucial properties are missing entirely)
	if (!bFoundDataTable)
	{
		Result.ValidationWarnings.Add(TEXT("No UDataTable reference property found in this Blueprint class."));
	}

	if (!bFoundCurveTable)
	{
		Result.ValidationWarnings.Add(TEXT("No UCurveTable reference property found in this Blueprint class."));
	}

	// If there are fatal errors, mark invalid
	if (Result.ValidationErrors.Num() > 0)
	{
		Result.bIsValid = false;
	}

	return Result;
}


#pragma endregion

#pragma region ActinTabFunction
bool FKaliToolsModule::ParseBugItString(const FString& InString, FVector& OutLocation, FRotator& OutRotation)
{
	FString Command = InString;
	if (Command.StartsWith(TEXT("BugItGo")))
	{
		Command.RightChopInline(7);
	}

	TArray<FString> Tokens;
	Command.ParseIntoArrayWS(Tokens);

	if (Tokens.Num() >= 6)
	{
		OutLocation.X = FCString::Atof(*Tokens[0]);
		OutLocation.Y = FCString::Atof(*Tokens[1]);
		OutLocation.Z = FCString::Atof(*Tokens[2]);

		OutRotation.Pitch = FCString::Atof(*Tokens[3]);
		OutRotation.Yaw = FCString::Atof(*Tokens[4]);
		OutRotation.Roll = FCString::Atof(*Tokens[5]);
		return true;
	}
	return false;
}

void FKaliToolsModule::LoadWorldPartitionPlaces(const TArray<FString>& RawStrings, TArray<FLoadedPlaceData>& OutLoadedPlaces)
{
	UWorld* World = GEditor ? GEditor->GetEditorWorldContext().World() : nullptr;
	if (!World)
	{
		return;
	}

	UWorldPartitionEditorSubsystem* WPEditorSubsystem = GEditor->GetEditorSubsystem<UWorldPartitionEditorSubsystem>();
	const FVector BoxExtent(5000.0f, 5000.0f, 5000.0f); // 100 meters square

	// Unload previously streamed-in regions before loading the new set,
	// so repeated "Load Places" clicks don't accumulate loaded WP cells.
	if (WPEditorSubsystem && PreviouslyLoadedRegions.Num() > 0)
	{
		WPEditorSubsystem->UnloadSelectedRegions(PreviouslyLoadedRegions);
		PreviouslyLoadedRegions.Empty();
	}

	for (int32 Index = 0; Index < RawStrings.Num(); ++Index)
	{
		FVector Location;
		FRotator Rotation;

		if (ParseBugItString(RawStrings[Index], Location, Rotation))
		{
			FLoadedPlaceData PlaceData;
			PlaceData.PlaceName = FString::Printf(TEXT("Place %d"), Index + 1);
			PlaceData.Location = Location;
			PlaceData.Rotation = Rotation;
			OutLoadedPlaces.Add(PlaceData);

			if (WPEditorSubsystem)
			{
				FBox TargetBox(Location - BoxExtent, Location + BoxExtent);
				WPEditorSubsystem->LoadSelectedRegions({ TargetBox });
				PreviouslyLoadedRegions.Add(TargetBox);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("[KaliTools] Failed to parse BugItGo string at index %d: %s"), Index, *RawStrings[Index]);
		}
	}
}

void FKaliToolsModule::TeleportToLocation(const FVector& Location, const FRotator& Rotation)
{
	if (!GEditor)
	{
		return;
	}

	FLevelEditorModule& LevelEditorModule = FModuleManager::GetModuleChecked<FLevelEditorModule>("LevelEditor");
	TSharedPtr<ILevelEditor> LevelEditor = LevelEditorModule.GetFirstLevelEditor();
	if (LevelEditor.IsValid())
	{
		TSharedPtr<SLevelViewport> ActiveViewport = LevelEditor->GetActiveViewportInterface();
		if (ActiveViewport.IsValid())
		{
			// FEditorViewportClient is the true type returned by GetAssetViewportClient()
			FEditorViewportClient& ViewportClient = ActiveViewport->GetAssetViewportClient();
			ViewportClient.SetViewLocation(Location);
			ViewportClient.SetViewRotation(Rotation);
			ViewportClient.Invalidate();
		}
	}
}
#pragma endregion

void FKaliToolsModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FKaliToolsModule, KaliTools)