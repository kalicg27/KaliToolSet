#include "SlateWidgets/ImplBlueprintSetUp.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Input/SButton.h"

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

void SImplBlueprintSetUp::RefreshListView()
{
	if (AssetListView.IsValid())
	{
		AssetListView->RequestListRefresh();
	}
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

		// --------------------------------------------------
		// 1. Validate Selected Asset Button (Primary Action)
		// --------------------------------------------------
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

	// --------------------------------------------------
	// 2. Secondary Utility Buttons Horizontal Row
	// --------------------------------------------------
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

	// --------------------------------------------------
	// 3. Main Start BP Setup Button
	// --------------------------------------------------
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

FReply SImplBlueprintSetUp::OnFetchAssetParametersClicked()
{
	//if (!AssetParametersBox.IsValid()) return FReply::Handled();

	//// Clear previous parameter widgets
	//AssetParametersBox->ClearChildren();

	//// Example parameter: "SomeParameter" with options [Red, Yellow, Black]
	//TArray<FString> ColorOptions = { TEXT("Red"), TEXT("Yellow"), TEXT("Black") };

	//// Create shared pointers for Slate SComboBox
	//TSharedPtr<TArray<TSharedPtr<FString>>> OptionItems = MakeShared<TArray<TSharedPtr<FString>>>();
	//for (const FString& Option : ColorOptions)
	//{
	//	OptionItems->Add(MakeShared<FString>(Option));
	//}

	//// Add a parameter row into AssetParametersBox
	//AssetParametersBox->AddSlot()
	//	.AutoHeight()
	//	.Padding(0.0f, 4.0f)
	//	[
	//		SNew(SHorizontalBox)

	//			// Property Label
	//			+ SHorizontalBox::Slot()
	//			.FillWidth(0.4f)
	//			.VAlign(VAlign_Center)
	//			[
	//				SNew(STextBlock)
	//					.Text(FText::FromString(TEXT("SomeParameter")))
	//			]

	//			// Dropdown Selector (Red / Yellow / Black)
	//			+ SHorizontalBox::Slot()
	//			.FillWidth(0.6f)
	//			[
	//				SNew(SComboBox<TSharedPtr<FString>>)
	//					.OptionsSource(OptionItems.Get())
	//					.OnGenerateWidget(this, &SImplBlueprintSetUp::MakeOptionComboWidget)
	//					.OnSelectionChanged_Lambda([](TSharedPtr<FString> NewValue, ESelectInfo::Type)
	//						{
	//							if (NewValue.IsValid())
	//							{
	//								UE_LOG(LogTemp, Log, TEXT("Selected Option: %s"), **NewValue);
	//							}
	//						})
	//					[
	//						SNew(STextBlock)
	//							.Text(FText::FromString(TEXT("Select Option...")))
	//					]
	//			]
	//	];

	return FReply::Handled();
}

TSharedRef<SWidget> SImplBlueprintSetUp::MakeOptionComboWidget(TSharedPtr<FString> InItem)
{
	return SNew(STextBlock).Text(FText::FromString(*InItem));
}

FReply SImplBlueprintSetUp::OnStartSettingUpBPClicked()
{
	// Execute backend apply, checkout, and save logic here
	UE_LOG(LogTemp, Log, TEXT("Starting BP setup and auto-checkout..."));
	return FReply::Handled();
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
        FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("No asset selected in Content Browser.")), FText::FromString(TEXT("Validation Failure")));
        return FReply::Handled();
    }

    UBlueprint* SelectedBP = Cast<UBlueprint>(SelectedAssets[0]);
    if (!SelectedBP)
    {
        FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Selected asset is not a Blueprint.")), FText::FromString(TEXT("Validation Failure")));
        return FReply::Handled();
    }

    FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Asset validation successful!")), FText::FromString(TEXT("Validation Passed")));
    return FReply::Handled();
}

FReply SImplBlueprintSetUp::OnCopySelectedClicked()
{
    TArray<UObject*> SelectedAssets = UEditorUtilityLibrary::GetSelectedAssets();
    if (SelectedAssets.Num() > 0 && SelectedAssets[0]->IsA<UBlueprint>())
    {
        // Extract values from selected Blueprint and update UI visual state directly here
        // e.g., ResourceTypeComboBox->SetSelectedItem(...);
    }

    return FReply::Handled();
}

FReply SImplBlueprintSetUp::OnStartBPSetupClicked()
{
    // Gather all parameters currently populated in the UI
    FKaliBlueprintSetupData DataToApply = GatherCurrentUIData();

    // Pass data payload directly to module engine handler (without applying inside this Slate widget)
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