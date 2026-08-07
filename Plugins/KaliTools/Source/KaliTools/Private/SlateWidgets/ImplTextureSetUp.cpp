#include "SlateWidgets/ImplTextureSetUp.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/SBoxPanel.h"
#include "Selection.h"
#include "Editor.h"
#include "Engine/Blueprint.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "AssetRegistry/AssetData.h"

#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"

#include "SourceControlHelpers.h"
#include "FileHelpers.h" // For FEditorFileUtils
#include "UObject/SavePackage.h"

#include "ISourceControlModule.h"
#include "ISourceControlProvider.h"

void SImplTextureSetUp::Construct(const FArguments& InArgs)
{
    // Define exact property names to monitor
    // Note: FName("InitialLifeSpan") works for both C++ properties and Blueprint variables
    TrackedProperties.Empty();
    TrackedProperties.Add({ FName("InitialLifeSpan"), FString("0.0") });
    TrackedProperties.Add({ FName("CustomTimeDilation"), FString("1.0") });

    ChildSlot
        [
            SNew(SVerticalBox)

                + SVerticalBox::Slot()
                .AutoHeight()
                .Padding(5)
                [
                    SNew(SButton)
                        .Text(FText::FromString("Fetch Selected Blueprint Parameters"))
                        .OnClicked(this, &SImplTextureSetUp::OnFetchPressed)
                ]

                // Container that dynamically holds property rows
                + SVerticalBox::Slot()
                .AutoHeight()
                .Padding(5)
                [
                    SAssignNew(DynamicRowsContainer, SVerticalBox)
                ]

                + SVerticalBox::Slot()
                .AutoHeight()
                .Padding(5)
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
        DynamicRowsContainer->AddSlot()
            .AutoHeight()
            .Padding(2)
            [
                SNew(SHorizontalBox)

                    // Property Label
                    + SHorizontalBox::Slot()
                    .AutoWidth()
                    .Padding(5)
                    [
                        SNew(STextBlock)
                            .Text(FText::FromName(Row.PropertyName))
                    ]

                    // Input Field
                    + SHorizontalBox::Slot()
                    .FillWidth(1.0f)
                    .Padding(5)
                    [
                        SAssignNew(Row.TextBoxWidget, SEditableTextBox)
                            .Text(FText::FromString(Row.CurrentValue))
                    ]
            ];
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
        // Check if the asset is a Blueprint
        if (AssetData.IsInstanceOf(UBlueprint::StaticClass()))
        {
            // Force load the asset if it isn't in memory yet
            TargetBP = Cast<UBlueprint>(AssetData.GetAsset());
            if (TargetBP)
            {
                break;
            }
        }
    }

    if (!TargetBP || !TargetBP->GeneratedClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("[Slate] Please select a Blueprint asset in the Content Browser!"));
        return FReply::Handled();
    }

    SelectedBlueprint = TargetBP;

    // 3. Get the Class Default Object (CDO)
    UObject* CDO = TargetBP->GeneratedClass->GetDefaultObject();

    if (CDO)
    {
        for (FPropertyRowData& Row : TrackedProperties)
        {
            // Look up the property in the Blueprint hierarchy
            FProperty* Property = FindFProperty<FProperty>(TargetBP->GeneratedClass, Row.PropertyName);

            if (Property)
            {
                const void* ValuePtr = Property->ContainerPtrToValuePtr<void>(CDO);

                FString ExportedText;
                Property->ExportTextItem_Direct(ExportedText, ValuePtr, nullptr, CDO, PPF_None);

                Row.CurrentValue = ExportedText;

                // Push updated value directly to the UI
                if (Row.TextBoxWidget.IsValid())
                {
                    Row.TextBoxWidget->SetText(FText::FromString(ExportedText));
                }

                UE_LOG(LogTemp, Log, TEXT("[Slate] Fetched '%s': %s"), *Row.PropertyName.ToString(), *ExportedText);
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("[Slate] Property '%s' not found on '%s'!"),
                    *Row.PropertyName.ToString(), *TargetBP->GetName());
            }
        }
    }

    return FReply::Handled();
}

FReply SImplTextureSetUp::OnApplyPressed()
{
    if (!SelectedBlueprint.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("[Slate] No Blueprint selected. Click 'Fetch' first."));
        return FReply::Handled();
    }

    UBlueprint* BP = SelectedBlueprint.Get();
    if (!BP || !BP->GeneratedClass) return FReply::Handled();

    UObject* CDO = BP->GeneratedClass->GetDefaultObject();
    if (!CDO) return FReply::Handled();

    UPackage* Package = BP->GetOutermost();
    if (!Package) return FReply::Handled();

    // 1. Begin Undo/Redo Transaction
    GEditor->BeginTransaction(FText::FromString("Modify Blueprint CDO Values"));

    CDO->Modify();
    BP->Modify();

    bool bPropertyChanged = false;

    for (FPropertyRowData& Row : TrackedProperties)
    {
        if (!Row.TextBoxWidget.IsValid()) continue;

        FProperty* Property = FindFProperty<FProperty>(BP->GeneratedClass, Row.PropertyName);
        if (Property)
        {
            void* ValuePtr = Property->ContainerPtrToValuePtr<void>(CDO);
            FString NewTextValue = Row.TextBoxWidget->GetText().ToString();

            // Import string value back to CDO memory
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
        FKismetEditorUtilities::CompileBlueprint(BP);
    }

    GEditor->EndTransaction();

    // 3. Save & Automatic Source Control Checkout
    if (bPropertyChanged)
    {
        Package->MarkPackageDirty();

        TArray<UPackage*> PackagesToSave;
        PackagesToSave.Add(Package);

        // Param 2: bCheckDirty (false)
        // Param 3: bPromptToSave (false = saves silently without prompt)
        // This function automatically handles Source Control Checkout under the hood!
        FEditorFileUtils::EPromptReturnCode SaveResult = FEditorFileUtils::PromptForCheckoutAndSave(PackagesToSave, false, false);

        if (SaveResult == FEditorFileUtils::EPromptReturnCode::PR_Success)
        {
            UE_LOG(LogTemp, Log, TEXT("[Slate] Successfully saved Blueprint asset: %s"), *BP->GetName());
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("[Slate] Save/Checkout failed for: %s"), *BP->GetName());
        }
    }

    return FReply::Handled();
}