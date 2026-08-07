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
								.Text(FText::FromString(TEXT("Blueprint SetUp Assets")))
								.Font(TitleFont)
								.ColorAndOpacity(FLinearColor::White)
						]
						
				]

			// 2. Asset List View Container
			+ SVerticalBox::Slot()
				.FillHeight(1.0f)
				.Padding(5.0f)
				[
					SAssignNew(AssetListView, SListView<TSharedPtr<FAssetData>>)
						.ListItemsSource(&StoredAssets)
						.OnGenerateRow(this, &SImplBlueprintSetUp::OnGenerateAssetRow)
				]
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