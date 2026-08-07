#include "SlateWidgets/ImplToolsWidget.h"
#include "SlateBasics.h"
#include "DebugHeader.h"
#include "KaliTools.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SBox.h"

#include "SlateWidgets/ImplBlueprintSetUp.h"
#include "SlateWidgets/ImplStaticMeshSetUp.h"
#include "SlateWidgets/ImplTextureSetUp.h"
#include "SlateWidgets/ImplActions.h"
#include "SlateWidgets/ImplVisuals.h"


void SImplToolsTab::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;

	// 1. Store incoming assets in member variable
	StoredAssetData = InArgs._AssetsDataToStore;

	FSlateFontInfo TitleTextFont = FCoreStyle::Get().GetFontStyle(FName("EmbossedText"));
	TitleTextFont.Size = 24;

	ChildSlot
		[
			SNew(SVerticalBox)

				// Title Banner
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(5.0f)
				[
					SNew(STextBlock)
						.Text(FText::FromString(TEXT("Implementation Tools")))
						.Font(TitleTextFont)
						.Justification(ETextJustify::Center)
						.ColorAndOpacity(FLinearColor::White)
				]

			// Sub-Tab Navigation Bar
			+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(5.0f)
				[
					SNew(SHorizontalBox)

						+ SHorizontalBox::Slot().FillWidth(1.0f).Padding(2.0f)
						[BuildTabButton(TEXT("Blueprint SetUp"), 0)]

						+ SHorizontalBox::Slot().FillWidth(1.0f).Padding(2.0f)
						[BuildTabButton(TEXT("Static Mesh SetUp"), 1)]

						+ SHorizontalBox::Slot().FillWidth(1.0f).Padding(2.0f)
						[BuildTabButton(TEXT("Texture SetUp"), 2)]

						+ SHorizontalBox::Slot().FillWidth(1.0f).Padding(2.0f)
						[BuildTabButton(TEXT("Actions"), 3)]

						+ SHorizontalBox::Slot().FillWidth(1.0f).Padding(2.0f)
						[BuildTabButton(TEXT("Visuals"), 4)]
				]

			// Sub-Tab Content Switcher
			+ SVerticalBox::Slot()
				.FillHeight(1.0f)
				.Padding(5.0f)
				[
					SAssignNew(TabWidgetSwitcher, SWidgetSwitcher)

						// Index 0: Blueprint SetUp Class
						+ SWidgetSwitcher::Slot()
						[
							SNew(SImplBlueprintSetUp)
								.SelectedAssets(StoredAssetData)
						]

						// Index 1: Static Mesh SetUp Class
						+ SWidgetSwitcher::Slot()
						[
							SNew(SImplStaticMeshSetUp)
								.SelectedAssets(StoredAssetData)
						]

						// Index 2: Texture SetUp Class
						+ SWidgetSwitcher::Slot()
						[
							SNew(SImplTextureSetUp)
								.SelectedAssets(StoredAssetData)
						]

						// Index 3: Actions Class
						+ SWidgetSwitcher::Slot()
						[
							SNew(SImplActions)
								
						]

						// Index 4: Visuals Class
						+ SWidgetSwitcher::Slot()
						[
							SNew(SImplVisuals)
								
						]
				]
			//+ SVerticalBox::Slot()
			//	.AutoHeight()
			//	[
			//		SNew(STextBlock)
			//			.Text(FText::FromString(TEXT("Implementetion Tools ")))
			//			.Font(TitleTextFont)
			//			.Justification(ETextJustify::Center)
			//			.ColorAndOpacity(FColor::White)
			//	]
			//+ SVerticalBox::Slot()
			//	.AutoHeight()
			//	[
			//		SNew(SHorizontalBox)
			//	]

			//	// for asset listing
			//	+ SVerticalBox::Slot()
			//	.VAlign(VAlign_Fill)
			//	[
			//		SNew(SScrollBox)
			//			+ SScrollBox::Slot()
			//			[
			//				ConstructAssetListView()
			//			]
			//	]

			//+ SVerticalBox::Slot()
			//	.AutoHeight()
			//	[
			//		SNew(SHorizontalBox)
			//	]

		];
}

TSharedRef<SListView<TSharedPtr<FAssetData>>> SImplToolsTab::ConstructAssetListView()
{
	ConstructedAssetListView = SNew(SListView<TSharedPtr<FAssetData>>)
		.ItemHeight(24.f)
		.ListItemsSource(&StoredAssetData)
		.OnGenerateRow(this, &SImplToolsTab::OnGeneratedRowForList);

	return ConstructedAssetListView.ToSharedRef(); 
	
}

TSharedRef<ITableRow> SImplToolsTab::OnGeneratedRowForList(TSharedPtr<FAssetData> AssetDataToDisplay, const TSharedRef<STableViewBase>& OwnerTable)
{
	if (!AssetDataToDisplay.IsValid()) return SNew(STableRow<TSharedPtr< FAssetData>>, OwnerTable);

	const FString DisplayAssetClassName = AssetDataToDisplay->AssetClassPath.GetAssetName().ToString();
	const FString DisplayAssetName = AssetDataToDisplay->AssetName.ToString();

	FSlateFontInfo AssetClassNameFont = GetEmboseedTextFont();
	AssetClassNameFont.Size = 10;

	FSlateFontInfo AssetNameFont = GetEmboseedTextFont();
	AssetNameFont.Size = 12;
	TSharedRef < STableRow < TSharedPtr < FAssetData>>> ListViewRowWidget =
		SNew(STableRow<TSharedPtr< FAssetData>>, OwnerTable).Padding(FMargin(5.f))
		[
			
			SNew(SHorizontalBox)
				//first slot check box
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Center)
				.FillWidth(.05f)
				[
					ConstructCheckBox(AssetDataToDisplay)
				]

				//second slot for dipslayeing the asset class
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Fill)
				.FillWidth(.5f)
				[
					ConstructTextForRowWidget(DisplayAssetClassName, AssetClassNameFont)
				]

				//third slot for diplsaying asset name
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Fill)
				[
					ConstructTextForRowWidget(DisplayAssetName, AssetNameFont)
				]

				// fourth for button delete
				+ SHorizontalBox::Slot()
				.HAlign(HAlign_Right)
				.VAlign(VAlign_Fill)
				[
					ConstructButtonRowWidget(AssetDataToDisplay)
				]
		];
	return ListViewRowWidget;

}

TSharedRef<SCheckBox> SImplToolsTab::ConstructCheckBox(const TSharedPtr<FAssetData>& AssetDataToDisplay)
{
	TSharedRef<SCheckBox> ConstuctedCheckBox = SNew(SCheckBox)
		.Type(ESlateCheckBoxType::CheckBox)
		.OnCheckStateChanged(this, &SImplToolsTab::OnCheckBoxStateChanged, AssetDataToDisplay)
		.Visibility(EVisibility::Visible);
	return ConstuctedCheckBox;
}

void SImplToolsTab::OnCheckBoxStateChanged(ECheckBoxState NewState, TSharedPtr<FAssetData> AssetData)
{
	switch (NewState)
	{
	case ECheckBoxState::Unchecked:
		DebugHeader::Print(AssetData->AssetName.ToString() + TEXT(" is not checked"), FColor::Red);
		break;
		
	case ECheckBoxState::Checked:
		DebugHeader::Print(AssetData->AssetName.ToString() + TEXT(" is checked"), FColor::Green);
		break;
	case ECheckBoxState::Undetermined:
		break;

	default:
		break;

	}
	
}

TSharedRef<STextBlock> SImplToolsTab::ConstructTextForRowWidget(const FString& TextContent, const FSlateFontInfo& FontToUse)
{
	TSharedRef<STextBlock> ConstructedTextBlock = SNew(STextBlock)
		.Text(FText::FromString(TextContent))
		.Font(FontToUse)
		.ColorAndOpacity(FColor::White);
	return ConstructedTextBlock;
}

TSharedRef<SButton> SImplToolsTab::ConstructButtonRowWidget(const TSharedPtr<FAssetData>& AssetDataToDisplay)
{
	TSharedRef<SButton> ConstructedButton= SNew(SButton)
		.Text(FText::FromString(TEXT("Delete")))
		.OnClicked(this,&SImplToolsTab::OnDeleteButtonClicked, AssetDataToDisplay);

	return ConstructedButton;
}

FReply SImplToolsTab::OnDeleteButtonClicked(TSharedPtr<FAssetData> ClickedAssetData)
{
	//DebugHeader::Print(ClickedAssetData->AssetName.ToString() + TEXT(" is clicked"), FColor::Red);
	FKaliToolsModule& KaliToolsModule = FModuleManager::LoadModuleChecked<FKaliToolsModule>(TEXT("KaliTools"));
	const bool bAssetDeleted = KaliToolsModule.DeleteSingleAssetForList(*ClickedAssetData.Get());

	if (bAssetDeleted)
	{
		//rpinting finish notification
		FString NotifyMsg = FString::Printf(TEXT("Successfully deleted %s"), *ClickedAssetData->AssetName.ToString());
		DebugHeader::ShowNotifyInfo(NotifyMsg);
		//updateing the lsit source items 
		if (StoredAssetData.Contains(ClickedAssetData))
		{
			StoredAssetData.Remove(ClickedAssetData);
		}

		//refresh the list
		RefreshAssetListView();
	}
	

	return FReply::Handled();
}

void SImplToolsTab::RefreshAssetListView()
{
	if (ConstructedAssetListView.IsValid())
	{
		ConstructedAssetListView->RebuildList();
	}
}

FReply SImplToolsTab::OnSubTabClicked(int32 TabIndex)
{
	if (TabWidgetSwitcher.IsValid())
	{
		TabWidgetSwitcher->SetActiveWidgetIndex(TabIndex);
	}
	return FReply::Handled();
}

TSharedRef<SWidget> SImplToolsTab::BuildTabButton(const FString& TabName, int32 TabIndex)
{
	return SNew(SButton)
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		.OnClicked(this, &SImplToolsTab::OnSubTabClicked, TabIndex)
		[
			SNew(STextBlock)
				.Text(FText::FromString(TabName))
		];
}