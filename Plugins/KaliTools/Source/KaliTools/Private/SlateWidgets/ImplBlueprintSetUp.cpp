#include "SlateWidgets/ImplBlueprintSetUp.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Input/SButton.h"
#include "EditorUtilityLibrary.h"
#include "DebugHeader.h"


void SImplBlueprintSetUp::Construct(const FArguments& InArgs)
{
	// Store the assets passed into .SelectedAssets(Assets)
	StoredAssets = InArgs._SelectedAssets;

	FSlateFontInfo TitleFont = FCoreStyle::Get().GetFontStyle(FName("EmbossedText"));
	TitleFont.Size = 16;

	ChildSlot
		[
			SNew(SVerticalBox)

				// 1. Header Section
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(10.0f)
				[
					SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.FillWidth(1.0f)
						[
							SNew(STextBlock)
								.Text(FText::FromString(TEXT("Blueprint SetUp Assets Main")))
								.Font(TitleFont)
								.ColorAndOpacity(FLinearColor::White)
						]
						
				]
			// all contetnt is here
			+ SVerticalBox::Slot()
				.FillHeight(1.0f)
				.Padding(5.0f)
				[
					SNew(SHorizontalBox)

						// Left Side: Asset Part (Info, Fetch Button, Dynamic Parameters)
						+ SHorizontalBox::Slot()
						.FillWidth(0.2f)
						.Padding(5.0f)
						[
							ConstructAssetPanel()
						]

						// Right Side: Main Actions Part (Setup Button, Global Options)
						+ SHorizontalBox::Slot()
						.FillWidth(0.5f)
						.Padding(5.0f)
						[
							ConstructMainActionsPanel()
						]
				]

			
			// demo Asset List View Container
			//+ SVerticalBox::Slot()
			//	.FillHeight(1.0f)
			//	.Padding(5.0f)
			//	[
			//		SAssignNew(AssetListView, SListView<TSharedPtr<FAssetData>>)
			//			.ListItemsSource(&StoredAssets)
			//			.OnGenerateRow(this, &SImplBlueprintSetUp::OnGenerateAssetRow)
			//	]
		];
}

TSharedRef<ITableRow> SImplBlueprintSetUp::OnGenerateAssetRow(TSharedPtr<FAssetData> AssetDataToDisplay, const TSharedRef<STableViewBase>& OwnerTable)
{
	if (!AssetDataToDisplay.IsValid())
	{
		return SNew(STableRow<TSharedPtr<FAssetData>>, OwnerTable);
	}

	FString AssetName = AssetDataToDisplay->AssetName.ToString();
	FString AssetClass = AssetDataToDisplay->AssetClassPath.GetAssetName().ToString();
	FString PackagePath = AssetDataToDisplay->PackagePath.ToString();

	return SNew(STableRow<TSharedPtr<FAssetData>>, OwnerTable)
		.Padding(FMargin(4.0f))
		[
			SNew(SHorizontalBox)

				// Asset Class Name
				+ SHorizontalBox::Slot()
				.FillWidth(0.2f)
				.VAlign(VAlign_Center)
				[
					SNew(STextBlock)
						.Text(FText::FromString(AssetClass))
						.ColorAndOpacity(FLinearColor::Green)
				]

				// Asset Name
				+ SHorizontalBox::Slot()
				.FillWidth(0.4f)
				.VAlign(VAlign_Center)
				[
					SNew(STextBlock)
						.Text(FText::FromString(AssetName))
						.ColorAndOpacity(FLinearColor::White)
				]

				// Folder Path
				+ SHorizontalBox::Slot()
				.FillWidth(0.4f)
				.VAlign(VAlign_Center)
				[
					SNew(STextBlock)
						.Text(FText::FromString(PackagePath))
						.ColorAndOpacity(FLinearColor::Gray)
				]
		];
}

TSharedRef<SWidget> SImplBlueprintSetUp::ConstructAssetPanel()
{
	return SNew(SVerticalBox)

		// 1. Language Button Row at top
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0.0f, 0.0f, 0.0f, 6.0f)
		[
			SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew(SButton)
						.OnClicked(this, &SImplBlueprintSetUp::OnLanguageButtonClicked)
						[
							SNew(STextBlock)
								.Text(FText::FromString(TEXT("English")))
						]
				]
		]
	//here is should show the bp type and name 
	// Expandable Area 1: "Reparent"
	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0.0f, 4.0f)
		[
			SNew(SExpandableArea)
				.AreaTitle(FText::FromString(TEXT("Reparent")))
				.InitiallyCollapsed(false)
				.BodyContent()
				[
					SNew(SVerticalBox)

						// Asset Display Info
						+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(2.0f)
						[
							SNew(STextBlock)
								.Text(FText::FromString(TEXT("Asset Display Name / Details")))
						]

						// Parent Class Picker / Info
						+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(2.0f)
						[
							SNew(STextBlock)
								.Text(FText::FromString(TEXT("Selected Asset Parent Class")))
						]

						// Reparent Execution Button
						+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(0.0f, 6.0f)
						[
							SNew(SButton)
								.OnClicked(this, &SImplBlueprintSetUp::OnReparentButtonClicked)
								[
									SNew(STextBlock)
										.Text(FText::FromString(TEXT("Reparent Asset")))
								]
						]
				]
		]

	// 3. Expandable Area 2: "Presets"
	+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0.0f, 4.0f)
		[
			SNew(SExpandableArea)
				.AreaTitle(FText::FromString(TEXT("Presets")))
				.InitiallyCollapsed(false)
				.BodyContent()
				[
					SNew(SScrollBox)
						+ SScrollBox::Slot()
						[
							SNew(SVerticalBox)

								// --- Prop Preset Buttons ---
								+ SVerticalBox::Slot().AutoHeight().Padding(2.0f)
								[
									SNew(SButton)
										.OnClicked(this, &SImplBlueprintSetUp::OnPresetButtonClicked, FName("PropWood"))
										[SNew(STextBlock).Text(FText::FromString(TEXT("Prop Wood")))]
								]
								+ SVerticalBox::Slot().AutoHeight().Padding(2.0f)
								[
									SNew(SButton)
										.OnClicked(this, &SImplBlueprintSetUp::OnPresetButtonClicked, FName("PropMetal"))
										[SNew(STextBlock).Text(FText::FromString(TEXT("Prop Metal")))]
								]
								+ SVerticalBox::Slot().AutoHeight().Padding(2.0f)
								[
									SNew(SButton)
										.OnClicked(this, &SImplBlueprintSetUp::OnPresetButtonClicked, FName("PropStone"))
										[SNew(STextBlock).Text(FText::FromString(TEXT("Prop Stone")))]
								]

								// --- Resource Preset Buttons ---
								+ SVerticalBox::Slot().AutoHeight().Padding(2.0f)
								[
									SNew(SButton)
										.OnClicked(this, &SImplBlueprintSetUp::OnPresetButtonClicked, FName("ResourceWood"))
										[SNew(STextBlock).Text(FText::FromString(TEXT("Resource Wood")))]
								]
								+ SVerticalBox::Slot().AutoHeight().Padding(2.0f)
								[
									SNew(SButton)
										.OnClicked(this, &SImplBlueprintSetUp::OnPresetButtonClicked, FName("ResourceMetal"))
										[SNew(STextBlock).Text(FText::FromString(TEXT("Resource Metal")))]
								]
								+ SVerticalBox::Slot().AutoHeight().Padding(2.0f)
								[
									SNew(SButton)
										.OnClicked(this, &SImplBlueprintSetUp::OnPresetButtonClicked, FName("ResourceStone"))
										[SNew(STextBlock).Text(FText::FromString(TEXT("Resource Stone")))]
								]

								// --- Building Preset Buttons ---
								+ SVerticalBox::Slot().AutoHeight().Padding(2.0f)
								[
									SNew(SButton)
										.OnClicked(this, &SImplBlueprintSetUp::OnPresetButtonClicked, FName("BuildingWood"))
										[SNew(STextBlock).Text(FText::FromString(TEXT("Building Wood")))]
								]
								+ SVerticalBox::Slot().AutoHeight().Padding(2.0f)
								[
									SNew(SButton)
										.OnClicked(this, &SImplBlueprintSetUp::OnPresetButtonClicked, FName("BuildingMetal"))
										[SNew(STextBlock).Text(FText::FromString(TEXT("Building Metal")))]
								]
								+ SVerticalBox::Slot().AutoHeight().Padding(2.0f)
								[
									SNew(SButton)
										.OnClicked(this, &SImplBlueprintSetUp::OnPresetButtonClicked, FName("BuildingStone"))
										[SNew(STextBlock).Text(FText::FromString(TEXT("Building Stone")))]
								]

								// --- Checkbox: isArch ---
								+ SVerticalBox::Slot().AutoHeight().Padding(4.0f)
								[
									SNew(SCheckBox)
										[
											SNew(STextBlock)
												.Text(FText::FromString(TEXT("Is Arch")))
										]
								]
						]
				]
		];
}

TSharedRef<SWidget> SImplBlueprintSetUp::ConstructMainActionsPanel()
{
	return SNew(SVerticalBox)

		// should rousorce changing stuff
		
		//validate button
		// 
		+SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0.0f, 0.0f, 0.0f, 6.0f)
		[
			SNew(SButton)
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				.OnClicked(this, &SImplBlueprintSetUp::OnValidateAssetClicked)
				[
					SNew(STextBlock)
						.Text(FText::FromString(TEXT("Validate Selected Asset")))
				]
		]

	//copy or create  --------------------------------------------------
	+SVerticalBox::Slot()
		.AutoHeight()
		.Padding(0.0f, 0.0f, 0.0f, 6.0f)
		[
			SNew(SHorizontalBox)

				// Copy Selected BP's Parameters Button
				+ SHorizontalBox::Slot()
				.FillWidth(1.0f)
				.Padding(0.0f, 0.0f, 3.0f, 0.0f)
				[
					SNew(SButton)
						.HAlign(HAlign_Center)
						.OnClicked(this, &SImplBlueprintSetUp::OnCopySelectedClicked)
						[
							SNew(STextBlock)
								.Text(FText::FromString(TEXT("Copy Selected BP's Parameters")))
						]
				]

			// Create a new BP Button
			+ SHorizontalBox::Slot()
				.FillWidth(1.0f)
				.Padding(3.0f, 0.0f, 0.0f, 0.0f)
				[
					SNew(SButton)
						.HAlign(HAlign_Center)
						[
							SNew(STextBlock)
								.Text(FText::FromString(TEXT("Create a new BP...")))
						]
				]
		]

	
	// Main Start BP Setup Button
	
	+SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SButton)
				.HAlign(HAlign_Center)
				.OnClicked(this, &SImplBlueprintSetUp::OnStartBPSetupClicked)
				[
					SNew(STextBlock)
						.Text(FText::FromString(TEXT("Start BP Setup")))
				]
		];
}

TSharedRef<SWidget> SImplBlueprintSetUp::MakeOptionComboWidget(TSharedPtr<FString> InItem)
{
	return SNew(STextBlock).Text(FText::FromString(*InItem));
}

FReply SImplBlueprintSetUp::OnLanguageButtonClicked()
{
	return FReply::Handled();
}

FReply SImplBlueprintSetUp::OnReparentButtonClicked()
{
	return FReply::Handled();
}

FReply SImplBlueprintSetUp::OnPresetButtonClicked(FName PresetName)
{
	return FReply::Handled();
}

FReply SImplBlueprintSetUp::OnValidateAssetClicked()
{
	TArray<UObject*> SelectedAssets = UEditorUtilityLibrary::GetSelectedAssets();

	if (SelectedAssets.Num() == 0)
	{
		DebugHeader::ShowMsgDialog(EAppMsgType::Ok, TEXT("No asset selected in Content Browser."));
		DebugHeader::PrintLog(TEXT("Validation Failed: Selection array empty."));
		return FReply::Handled();
	}

	UBlueprint* SelectedBP = Cast<UBlueprint>(SelectedAssets[0]);
	if (!SelectedBP)
	{
		DebugHeader::ShowMsgDialog(EAppMsgType::Ok, TEXT("Selected asset is not a valid Blueprint."));
		DebugHeader::PrintLog(TEXT("Validation Failed: Target object is not a UBlueprint."));
		return FReply::Handled();
	}

	// Success notification via DebugHeader
	DebugHeader::ShowNotifyInfo(FString::Printf(TEXT("Asset Validated: %s"), *SelectedBP->GetName()));
	DebugHeader::PrintLog(TEXT("Validation Succeeded for: ") + SelectedBP->GetName());

	return FReply::Handled();
}

FReply SImplBlueprintSetUp::OnCopySelectedClicked()
{
	TArray<UObject*> SelectedAssets = UEditorUtilityLibrary::GetSelectedAssets();

	if (SelectedAssets.Num() == 0 || !SelectedAssets[0]->IsA<UBlueprint>())
	{
		DebugHeader::ShowNotifyInfo(TEXT("Failed to copy parameters: Invalid Blueprint selected."));
		return FReply::Handled();
	}

	// Fetch and populate parameters...
	DebugHeader::ShowNotifyInfo(TEXT("Parameters copied from selected Blueprint."));
	return FReply::Handled();
}

FReply SImplBlueprintSetUp::OnStartBPSetupClicked()
{
	FKaliBlueprintSetupData DataToApply = GatherCurrentUIData();

	DebugHeader::ShowNotifyInfo(TEXT("Executing BP Setup... Passing payload to plugin."));
	DebugHeader::PrintLog(TEXT("BP Setup started with Resource Type: ") + DataToApply.ResourceType);

	SendDataToModulePlugin(DataToApply);

	return FReply::Handled();
}

FKaliBlueprintSetupData SImplBlueprintSetUp::GatherCurrentUIData() const
{
    FKaliBlueprintSetupData Data;
    if (ResourceTypeComboBox.IsValid() && ResourceTypeComboBox->GetSelectedItem().IsValid())
    {
        Data.ResourceType = *ResourceTypeComboBox->GetSelectedItem();
    }
    return Data;
}

void SImplBlueprintSetUp::SendDataToModulePlugin(const FKaliBlueprintSetupData& Data)
{
    // Hand off payload to module plugin execution logic
    // FKaliToolSetModule::Get().ExecuteBlueprintSetup(Data);
}

void SImplBlueprintSetUp::RefreshListView()
{
	if (AssetListView.IsValid())
	{
		AssetListView->RequestListRefresh();
	}
}