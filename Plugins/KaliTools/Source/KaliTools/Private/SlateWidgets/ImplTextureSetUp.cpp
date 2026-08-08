#include "SlateWidgets/ImplTextureSetUp.h"
#include "KaliTools.h"
#include "Modules/ModuleManager.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/SBoxPanel.h"
#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"
#include "Engine/Blueprint.h"

void SImplTextureSetUp::Construct(const FArguments& InArgs)
{
    TrackedProperties.Empty();
    TrackedProperties.Add({ FName("InitialLifeSpan"), FString("0.0") });
    TrackedProperties.Add({ FName("CustomTimeDilation"), FString("1.0") });

    ChildSlot
        [
            SNew(SVerticalBox)

                + SVerticalBox::Slot()
                .AutoHeight()
                .Padding(5.0f)
                [
                    SNew(SButton)
                        .Text(FText::FromString("Fetch Selected Blueprint Parameters"))
                        .OnClicked(this, &SImplTextureSetUp::OnFetchPressed)
                ]

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
    if (!DynamicRowsContainer.IsValid())
    {
        return;
    }

    DynamicRowsContainer->ClearChildren();

    for (FPropertyRowData& Row : TrackedProperties)
    {
        DynamicRowsContainer->AddSlot()
            .AutoHeight()
            .Padding(2.0f)
            [
                SNew(SHorizontalBox)

                    + SHorizontalBox::Slot()
                    .AutoWidth()
                    .Padding(5.0f)
                    [
                        SNew(STextBlock)
                            .Text(FText::FromName(Row.PropertyName))
                    ]

                    + SHorizontalBox::Slot()
                    .FillWidth(1.0f)
                    .Padding(5.0f)
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
        // Safe class check for Blueprints in Content Browser
        UClass* AssetClass = AssetData.GetClass();
        if (AssetClass && AssetClass->IsChildOf(UBlueprint::StaticClass()))
        {
            TargetBP = Cast<UBlueprint>(AssetData.GetAsset());
            if (TargetBP)
            {
                break;
            }
        }
    }

    if (!TargetBP)
    {
        UE_LOG(LogTemp, Warning, TEXT("[Slate] Please select a valid Blueprint asset in the Content Browser!"));
        return FReply::Handled();
    }

    SelectedBlueprint = TargetBP;

    TArray<FName> NamesToFetch;
    for (const FPropertyRowData& Row : TrackedProperties)
    {
        NamesToFetch.Add(Row.PropertyName);
    }

    TMap<FName, FString> FetchedValues;
    if (FKaliToolsModule::FetchBlueprintProperties(TargetBP, NamesToFetch, FetchedValues))
    {
        for (FPropertyRowData& Row : TrackedProperties)
        {
            if (const FString* FoundVal = FetchedValues.Find(Row.PropertyName))
            {
                Row.CurrentValue = *FoundVal;
                if (Row.TextBoxWidget.IsValid())
                {
                    Row.TextBoxWidget->SetText(FText::FromString(*FoundVal));
                }
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

    TMap<FName, FString> ValuesToApply;
    for (FPropertyRowData& Row : TrackedProperties)
    {
        if (Row.TextBoxWidget.IsValid())
        {
            ValuesToApply.Add(Row.PropertyName, Row.TextBoxWidget->GetText().ToString());
        }
    }

    FKaliToolsModule::ApplyBlueprintProperties(SelectedBlueprint.Get(), ValuesToApply);

    return FReply::Handled();
}