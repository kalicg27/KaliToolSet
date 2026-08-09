#include "SlateWidgets/ImplTextureSetUp.h"
#include "KaliTools.h"
#include "Modules/ModuleManager.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/SBoxPanel.h"
#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"
#include "Engine/Blueprint.h"
#include "DebugHeader.h"

#include "ImplToolsSetTypes.h"   
#include "Engine/EngineTypes.h" 
#include "Engine/Engine.h"

#include "ClassIconFinder.h"
#include "Styling/SlateIconFinder.h"

void SImplTextureSetUp::Construct(const FArguments& InArgs)
{
    SelectedAssetsList = InArgs._SelectedAssets;
    TrackedProperties.Empty();

    // standart values
    TrackedProperties.Add({ FName("InitialLifeSpan"), FString("0.0"), EPropertyRowType::Text });
    TrackedProperties.Add({ FName("CustomTimeDilation"), FString("1.0"), EPropertyRowType::Text });

    //enum properties
    FImplToolsPropertyUtils::AddEnumProperty(TrackedProperties, FName("NetDormancy"), StaticEnum<ENetDormancy>(), TEXT("DORM_Awake"));
    FImplToolsPropertyUtils::AddEnumProperty(TrackedProperties, FName("PhysicsReplicationMode"), StaticEnum<EPhysicsReplicationMode>(), TEXT("Default"));
    FImplToolsPropertyUtils::AddEnumProperty(TrackedProperties, FName("SpawnCollisionHandlingMethod"), StaticEnum<ESpawnActorCollisionHandlingMethod>(), TEXT("AlwaysSpawn"));

    //curve tables
    UCurveTable* DefaultCurveTable = nullptr;
    FImplToolsPropertyUtils::AddCurveTableRowProperty(TrackedProperties, FName("BuildingResourceAmountOverride"), DefaultCurveTable, TEXT("None"));

    // FName Properties (Handled as editable text fields)
    TrackedProperties.Add({ FName("BuildingResourceAmountOverrideRowName"), FString("None"), EPropertyRowType::Text });
    TrackedProperties.Add({ FName("ItemAttributeInitCategory"), FString("None"), EPropertyRowType::Text });

    // custom data
    TrackedProperties.Add({ FName("AttributeInitKeys"), FString(""), EPropertyRowType::Text });
    TrackedProperties.Add({ FName("AttributeInitKeysArray"), FString(""), EPropertyRowType::Text });

    // Status & UI Initialization
    StatusText = FText::FromString("Select a Blueprint, Data Table, or Curve Table asset and click 'Fetch'.");
    FetchedAssetNameText = FText::FromString("No Asset Loaded");
    FetchedAssetIconBrush = FAppStyle::GetBrush("Icons.Help");
    ChildSlot
        [
            SNew(SVerticalBox)

                // Asset Header Display
                + SVerticalBox::Slot()
                .AutoHeight()
                .Padding(5.0f)
                [
                    SNew(SBorder)
                        .Padding(8.0f)
                        .BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
                        [
                            SNew(SHorizontalBox)

                                + SHorizontalBox::Slot()
                                .AutoWidth()
                                .VAlign(VAlign_Center)
                                .Padding(0.0f, 0.0f, 8.0f, 0.0f)
                                [
                                    SNew(SImage)
                                        .Image_Lambda([this]() { return FetchedAssetIconBrush; })
                                        .DesiredSizeOverride(FVector2D(24.0f, 24.0f))
                                ]

                                + SHorizontalBox::Slot()
                                .FillWidth(1.0f)
                                .VAlign(VAlign_Center)
                                [
                                    SNew(STextBlock)
                                        .Text_Lambda([this]() { return FetchedAssetNameText; })
                                        .Font(FAppStyle::GetFontStyle("PropertyWindow.BoldFont"))
                                ]
                        ]
                ]

            // Status Text
            + SVerticalBox::Slot()
                .AutoHeight()
                .Padding(5.0f)
                [
                    SAssignNew(StatusTextBlock, STextBlock)
                        .Text_Lambda([this]() { return StatusText; })
                        .ColorAndOpacity(FSlateColor(FLinearColor::Yellow))
                ]

                // Action Buttons
                + SVerticalBox::Slot()
                .AutoHeight()
                .Padding(5.0f)
                [
                    SNew(SButton)
                        .Text(FText::FromString("Validate Selected Asset"))
                        .OnClicked(this, &SImplTextureSetUp::OnValidatePressed)
                ]

                + SVerticalBox::Slot()
                .AutoHeight()
                .Padding(5.0f)
                [
                    SNew(SButton)
                        .Text(FText::FromString("Fetch Selected Asset Parameters"))
                        .OnClicked(this, &SImplTextureSetUp::OnFetchPressed)
                ]

                // Dynamic Rows Container
                + SVerticalBox::Slot()
                .AutoHeight()
                .Padding(5.0f)
                [
                    SAssignNew(DynamicRowsContainer, SVerticalBox)
                ]

                + SVerticalBox::Slot()
                .AutoHeight()
                .Padding(5.0f)
                [
                    SNew(SButton)
                        .Text(FText::FromString("Apply All Changes"))
                        .OnClicked(this, &SImplTextureSetUp::OnApplyPressed)
                ]
        ];

    RebuildPropertyRowsUI();
}

void SImplTextureSetUp::RebuildPropertyRowsUI()
{
    if (!DynamicRowsContainer.IsValid()) return;
    DynamicRowsContainer->ClearChildren();

    for (FPropertyRowData& Row : TrackedProperties)
    {
        TSharedPtr<SWidget> ValueInputWidget;

        if (Row.RowType == EPropertyRowType::Enum)
        {
            // Build SComboBox Dropdown for Enum Properties
            ValueInputWidget = SAssignNew(Row.ComboBoxWidget, SComboBox<TSharedPtr<FString>>)
                .OptionsSource(&Row.EnumOptions)
                .OnGenerateWidget(this, &SImplTextureSetUp::GenerateEnumComboWidget)
                .OnSelectionChanged(this, &SImplTextureSetUp::OnEnumSelectionChanged, Row.PropertyName)
                [
                    SNew(STextBlock)
                        .Text(this, &SImplTextureSetUp::GetCurrentEnumText, Row.PropertyName)
                ];
        }
        else
        {
            // Build SEditableTextBox for Standard Properties
            ValueInputWidget = SAssignNew(Row.TextBoxWidget, SEditableTextBox)
                .Text(FText::FromString(Row.CurrentValue));
        }

        DynamicRowsContainer->AddSlot()
            .AutoHeight()
            .Padding(2.0f)
            [
                SNew(SHorizontalBox)

                    + SHorizontalBox::Slot()
                    .AutoWidth()
                    .Padding(5.0f)
                    [
                        SNew(STextBlock).Text(FText::FromName(Row.PropertyName))
                    ]

                    + SHorizontalBox::Slot()
                    .FillWidth(1.0f)
                    .Padding(5.0f)
                    [
                        ValueInputWidget.ToSharedRef()
                    ]
            ];
    }
}

TSharedRef<SWidget> SImplTextureSetUp::GenerateEnumComboWidget(TSharedPtr<FString> InItem)
{
    return SNew(STextBlock).Text(FText::FromString(*InItem));
}

FText SImplTextureSetUp::GetCurrentEnumText(FName PropertyName) const
{
    for (const FPropertyRowData& Row : TrackedProperties)
    {
        if (Row.PropertyName == PropertyName)
        {
            return FText::FromString(Row.CurrentValue);
        }
    }
    return FText::GetEmpty();
}

void SImplTextureSetUp::OnEnumSelectionChanged(TSharedPtr<FString> NewValue, ESelectInfo::Type SelectInfo, FName PropertyName)
{
    if (!NewValue.IsValid()) return;

    for (FPropertyRowData& Row : TrackedProperties)
    {
        if (Row.PropertyName == PropertyName)
        {
            Row.CurrentValue = *NewValue;
            Row.SelectedEnumOption = NewValue;
            break;
        }
    }
}

void SImplTextureSetUp::UpdateFetchedAssetHeader(UBlueprint* InBlueprint)
{
    if (InBlueprint)
    {
        SelectedBlueprint = InBlueprint;
        FetchedAssetNameText = FText::FromString(InBlueprint->GetName());

        // 👈 Use .Get() on GeneratedClass to resolve to raw UClass*
        UClass* TargetClass = InBlueprint->GeneratedClass.Get() ? InBlueprint->GeneratedClass.Get() : InBlueprint->GetClass();

        FSlateIcon AssetIcon = FSlateIconFinder::FindIconForClass(TargetClass);

        FetchedAssetIconBrush = AssetIcon.GetIcon();
        if (!FetchedAssetIconBrush || FetchedAssetIconBrush == FAppStyle::GetDefaultBrush())
        {
            // Fallback for Blueprint assets
            FetchedAssetIconBrush = FAppStyle::GetBrush("ClassIcon.Blueprint");
        }
    }
    else
    {
        SelectedBlueprint.Reset();
        FetchedAssetNameText = FText::FromString("No Blueprint Loaded");
        FetchedAssetIconBrush = FAppStyle::GetBrush("Icons.Help");
    }
}

FReply SImplTextureSetUp::OnFetchPressed()
{
    FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");

    TArray<FAssetData> SelectedAssets;
    ContentBrowserModule.Get().GetSelectedAssets(SelectedAssets);

    UBlueprint* TargetBP = nullptr;
    for (const FAssetData& AssetData : SelectedAssets)
    {
        if (AssetData.GetClass() && AssetData.GetClass()->IsChildOf(UBlueprint::StaticClass()))
        {
            TargetBP = Cast<UBlueprint>(AssetData.GetAsset());
            if (TargetBP) break;
        }
    }

    if (!TargetBP)
    {
        UpdateFetchedAssetHeader(nullptr);
        StatusText = FText::FromString("No valid Blueprint selected in Content Browser!");
        return FReply::Handled();
    }

    // 1. Update UI Header (Icon + Asset Name)
    UpdateFetchedAssetHeader(TargetBP);

    // 2. Extract target property names
    TArray<FName> NamesToFetch;
    for (const FPropertyRowData& Row : TrackedProperties)
    {
        NamesToFetch.Add(Row.PropertyName);
    }

    // 3. Fetch reflection values from Blueprint
    TMap<FName, FString> FetchedValues;
    if (FKaliToolsModule::FetchBlueprintProperties(TargetBP, NamesToFetch, FetchedValues))
    {
        for (FPropertyRowData& Row : TrackedProperties)
        {
            if (const FString* FoundVal = FetchedValues.Find(Row.PropertyName))
            {
                Row.CurrentValue = *FoundVal;

                if (Row.RowType == EPropertyRowType::Text && Row.TextBoxWidget.IsValid())
                {
                    Row.TextBoxWidget->SetText(FText::FromString(*FoundVal));
                }
                else if (Row.RowType == EPropertyRowType::Enum && Row.ComboBoxWidget.IsValid())
                {
                    // Match and select item in dropdown menu
                    for (const TSharedPtr<FString>& Option : Row.EnumOptions)
                    {
                        if (Option.IsValid() && *Option == *FoundVal)
                        {
                            Row.SelectedEnumOption = Option;
                            Row.ComboBoxWidget->SetSelectedItem(Option);
                            break;
                        }
                    }
                }
            }
        }

        StatusText = FText::FromString(FString::Printf(TEXT("Successfully fetched properties from '%s'!"), *TargetBP->GetName()));
    }
    else
    {
        StatusText = FText::FromString(FString::Printf(TEXT("Failed to read properties from '%s'."), *TargetBP->GetName()));
    }

    return FReply::Handled();
}

FReply SImplTextureSetUp::OnApplyPressed()
{
    FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");

    TArray<FAssetData> SelectedAssets;
    ContentBrowserModule.Get().GetSelectedAssets(SelectedAssets);

    // 1. Guard against no selection
    if (SelectedAssets.Num() == 0)
    {
        StatusText = FText::FromString("No asset selected in Content Browser!");
        if (StatusTextBlock.IsValid()) StatusTextBlock->SetColorAndOpacity(FSlateColor(FLinearColor::Red));

        DebugHeader::ShowMsgDialog(EAppMsgType::Ok, TEXT("Please select a Blueprint asset before applying changes."), true);
        return FReply::Handled();
    }

    const FAssetData& CurrentAssetData = SelectedAssets[0];
    FString AssetName = CurrentAssetData.AssetName.ToString();

    // 2. Reject non-Blueprint assets (e.g. StaticMeshes, Textures)
    UClass* AssetClass = CurrentAssetData.GetClass();
    bool bIsBlueprint = AssetClass && AssetClass->IsChildOf(UBlueprint::StaticClass());

    if (!bIsBlueprint)
    {
        // Reset stored reference so stale Blueprints are never modified
        SelectedBlueprint.Reset();

        StatusText = FText::FromString(FString::Printf(TEXT("Cannot apply changes: '%s' is not a Blueprint!"), *AssetName));
        if (StatusTextBlock.IsValid()) StatusTextBlock->SetColorAndOpacity(FSlateColor(FLinearColor::Red));

        // Pop up warning dialog
        FString ErrorMsg = FString::Printf(TEXT("Selected asset '%s' is not a Blueprint (Class: %s). Changes were NOT applied."), *AssetName, *AssetClass->GetName());
        DebugHeader::ShowMsgDialog(EAppMsgType::Ok, ErrorMsg, true);

        return FReply::Handled();
    }

    // 3. Update active target pointer to current selection
    UBlueprint* TargetBP = Cast<UBlueprint>(CurrentAssetData.GetAsset());
    SelectedBlueprint = TargetBP;

    if (!TargetBP)
    {
        DebugHeader::ShowMsgDialog(EAppMsgType::Ok, TEXT("Failed to load selected Blueprint asset."), true);
        return FReply::Handled();
    }

    // 4. Gather property values from UI
    TMap<FName, FString> ValuesToApply;
    for (FPropertyRowData& Row : TrackedProperties)
    {
        // Grab value directly from text box or stored current value
        if (Row.TextBoxWidget.IsValid())
        {
            Row.CurrentValue = Row.TextBoxWidget->GetText().ToString();
        }
        ValuesToApply.Add(Row.PropertyName, Row.CurrentValue);
    }

    // 5. Apply properties safely
    bool bSuccess = FKaliToolsModule::ApplyBlueprintProperties(TargetBP, ValuesToApply);

    if (bSuccess)
    {
        StatusText = FText::FromString(FString::Printf(TEXT("Successfully updated '%s'! ✅"), *AssetName));
        if (StatusTextBlock.IsValid()) StatusTextBlock->SetColorAndOpacity(FSlateColor(FLinearColor::Green));
    }

    return FReply::Handled();
}

FReply SImplTextureSetUp::OnValidatePressed()
{
    FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");

    TArray<FAssetData> SelectedAssets;
    ContentBrowserModule.Get().GetSelectedAssets(SelectedAssets);

    if (SelectedAssets.Num() == 0)
    {
        StatusText = FText::FromString("No asset selected in Content Browser!");
        if (StatusTextBlock.IsValid()) StatusTextBlock->SetColorAndOpacity(FSlateColor(FLinearColor::Red));
        return FReply::Handled();
    }

    const FAssetData& AssetData = SelectedAssets[0];
    FString AssetName = AssetData.AssetName.ToString();

    // Check if the selected asset is a Blueprint
    UClass* AssetClass = AssetData.GetClass();
    bool bIsBlueprint = AssetClass && AssetClass->IsChildOf(UBlueprint::StaticClass());

    if (!bIsBlueprint)
    {
        // 👈 Display target asset name and state directly in the UI
        StatusText = FText::FromString(FString::Printf(TEXT("Selected asset '%s' is not a Blueprint!"), *AssetName));
        if (StatusTextBlock.IsValid()) StatusTextBlock->SetColorAndOpacity(FSlateColor(FLinearColor::Red));

        UE_LOG(LogTemp, Warning, TEXT("[KaliValidator] Asset '%s' is not a Blueprint."), *AssetName);
        return FReply::Handled();
    }

    // Asset is a valid Blueprint -> Proceed with backend validation
    UBlueprint* TargetBP = Cast<UBlueprint>(AssetData.GetAsset());
    FKaliValidationResult Result = FKaliToolsModule::ValidateBlueprintSetup(TargetBP);

    if (Result.bIsValid && Result.ValidationWarnings.Num() == 0)
    {
        StatusText = FText::FromString(FString::Printf(TEXT("'%s' is a valid Blueprint! ✅"), *AssetName));
        if (StatusTextBlock.IsValid()) StatusTextBlock->SetColorAndOpacity(FSlateColor(FLinearColor::Green));
    }
    else
    {
        StatusText = FText::FromString(FString::Printf(TEXT("'%s' is a Blueprint with %d warnings! ⚠️"), *AssetName, Result.ValidationWarnings.Num()));
        if (StatusTextBlock.IsValid()) StatusTextBlock->SetColorAndOpacity(FSlateColor(FLinearColor::Yellow));
    }

    return FReply::Handled();
}
