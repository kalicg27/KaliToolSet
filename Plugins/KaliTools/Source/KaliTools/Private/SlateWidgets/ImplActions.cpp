#include "SlateWidgets/ImplActions.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "KaliTools.h"
void SImplActions::Construct(const FArguments& InArgs)
{
	ChildSlot
		[
			SNew(SScrollBox)
				+ SScrollBox::Slot()
				.Padding(10.0f)
				[
					SNew(SVerticalBox)

						//category input for cordinates
						+SVerticalBox::Slot()
						.AutoHeight()
						.Padding(0.0f, 0.0f, 0.0f, 5.0f)
						[
							SNew(SHorizontalBox)
								+ SHorizontalBox::Slot()
								.FillWidth(1.0f)
								.VAlign(VAlign_Center)
								[
									SNew(STextBlock)
										.Text(NSLOCTEXT("KaliTools", "Cat1Title", "1. Place Initialization"))
										.Font(FCoreStyle::GetDefaultFontStyle("Bold", 12))
								]
								// "+" Button located on the right side of the header area
								+ SHorizontalBox::Slot()
								.AutoWidth()
								[
									SNew(SButton)
										.Text(FText::FromString(TEXT("  + Add Place  ")))
										.OnClicked(this, &SImplActions::OnAddPlaceInputClicked)
								]
						]

					// Container for dynamic dynamic input rows
					+ SVerticalBox::Slot()
						.AutoHeight()
						[
							SAssignNew(InputListContainer, SVerticalBox)
						]

						// Load Places Trigger Button
						+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(0.0f, 8.0f, 0.0f, 15.0f)
						[
							SNew(SButton)
								.HAlign(HAlign_Center)
								.Text(FText::FromString(TEXT("Load Places")))
								.OnClicked(this, &SImplActions::OnLoadPlacesClicked)
						]

						// Separator Line
						+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(0.0f, 5.0f)
						[
							SNew(STextBlock).Text(FText::FromString(TEXT("--------------------------------------------------")))
						]

						// world partion level show
						+SVerticalBox::Slot()
						.AutoHeight()
						.Padding(0.0f, 5.0f)
						[
							SNew(STextBlock)
								.Text(NSLOCTEXT("KaliTools", "Cat2Title", "2. World Partition Overview"))
								.Font(FCoreStyle::GetDefaultFontStyle("Bold", 12))
						]
						+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(0.0f, 2.0f, 0.0f, 15.0f)
						[
							SNew(STextBlock)
								.Text(FText::FromString(TEXT("World Partition system active. Cell streaming range: 100 meters per loaded target.")))
								.ColorAndOpacity(FSlateColor(FLinearColor::SubduedGray))
						]

						// Separator Line
						+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(0.0f, 5.0f)
						[
							SNew(STextBlock).Text(FText::FromString(TEXT("--------------------------------------------------")))
						]

						// ==========================================
						// CATEGORY 3: LOADED PLACES LIST & GO BUTTONS
						// ==========================================
						+SVerticalBox::Slot()
						.AutoHeight()
						.Padding(0.0f, 5.0f)
						[
							SNew(STextBlock)
								.Text(NSLOCTEXT("KaliTools", "Cat3Title", "3. Loaded Places"))
								.Font(FCoreStyle::GetDefaultFontStyle("Bold", 12))
						]

						// Container for loaded place rows with GO buttons
						+ SVerticalBox::Slot()
						.AutoHeight()
						[
							SAssignNew(LoadedListContainer, SVerticalBox)
						]
				]
		];

	// Instantiate the first place entry by default
	OnAddPlaceInputClicked();
}

Reply SImplActions::OnAddPlaceInputClicked()
{
	TSharedPtr<FQuickTeleportInputItem> NewItem = MakeShared<FQuickTeleportInputItem>();
	NewItem->DefaultName = FString::Printf(TEXT("Place %d"), InputItems.Num() + 1);
	NewItem->CoordinateInput = SNew(SEditableTextBox)
		.HintText(FText::FromString(TEXT("Paste BugItGo string (e.g. BugItGo X Y Z Pitch Yaw Roll)")));

	InputItems.Add(NewItem);
	RefreshInputList();

	return FReply::Handled();
}

FReply SImplActions::OnRemovePlaceInputClicked(TSharedPtr<FQuickTeleportInputItem> ItemToRemove)
{
	InputItems.Remove(ItemToRemove);
	RefreshInputList();
	return FReply::Handled();
}

void SImplActions::RefreshInputList()
{
	InputListContainer->ClearChildren();

	for (int32 i = 0; i < InputItems.Num(); ++i)
	{
		InputItems[i]->DefaultName = FString::Printf(TEXT("Place %d"), i + 1);
		InputListContainer->AddSlot()
			.AutoHeight()
			.Padding(0.0f, 2.0f)
			[
				BuildInputRow(InputItems[i], i)
			];
	}
}

TSharedRef<SWidget> SImplActions::BuildInputRow(TSharedPtr<FQuickTeleportInputItem> Item, int32 Index)
{
	return SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(VAlign_Center)
		.Padding(0.0f, 0.0f, 10.0f, 0.0f)
		[
			SNew(STextBlock)
				.Text(FText::FromString(Item->DefaultName))
		]
		+ SHorizontalBox::Slot()
		.FillWidth(1.0f)
		.Padding(0.0f, 0.0f, 5.0f, 0.0f)
		[
			Item->CoordinateInput.ToSharedRef()
		]
		+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SButton)
				.Text(FText::FromString(TEXT(" - ")))
				.OnClicked(this, &SImplActions::OnRemovePlaceInputClicked, Item)
		];
}

FReply SImplActions::OnLoadPlacesClicked()
{
	TArray<FString> RawStrings;
	for (const auto& Item : InputItems)
	{
		if (Item->CoordinateInput.IsValid())
		{
			FString Text = Item->CoordinateInput->GetText().ToString().TrimStartAndEnd();
			if (!Text.IsEmpty())
			{
				RawStrings.Add(Text);
			}
		}
	}

	if (RawStrings.Num() == 0)
	{
		return FReply::Handled();
	}

	// Fetch module reference
	FKaliToolsModule& KaliModule = FModuleManager::GetModuleChecked<FKaliToolsModule>("KaliTools");

	// 1. Process World Partition loads and receive back parsed locations
	LoadedPlaces.Empty();
	KaliModule.LoadWorldPartitionPlaces(RawStrings, LoadedPlaces);

	// 2. Populate UI Category 3
	RefreshLoadedList();

	// 3. Automatically teleport to Place 1 upon pressing Load Places
	if (LoadedPlaces.Num() > 0)
	{
		KaliModule.TeleportToLocation(LoadedPlaces[0].Location, LoadedPlaces[0].Rotation);
	}

	return FReply::Handled();
}

// ----------------------------------------------------
// CATEGORY 3 IMPLEMENTATION
// ----------------------------------------------------

void SImplActions::RefreshLoadedList()
{
	LoadedListContainer->ClearChildren();

	if (LoadedPlaces.Num() == 0)
	{
		LoadedListContainer->AddSlot()
			.AutoHeight()
			.Padding(0.0f, 5.0f)
			[
				SNew(STextBlock)
					.Text(FText::FromString(TEXT("No places loaded yet. Click 'Load Places' above.")))
					.ColorAndOpacity(FSlateColor(FLinearColor(0.5f, 0.5f, 0.5f, 1.0f))) // Fixed color
			];
		return;
	}

	for (const FLoadedPlaceData& Place : LoadedPlaces)
	{
		LoadedListContainer->AddSlot()
			.AutoHeight()
			.Padding(0.0f, 2.0f)
			[
				BuildLoadedRow(Place)
			];
	}
}

TSharedRef<SWidget> SImplActions::BuildLoadedRow(const FLoadedPlaceData& PlaceData)
{
	FString DisplayCoord = FString::Printf(TEXT("(X=%.0f, Y=%.0f, Z=%.0f)"), PlaceData.Location.X, PlaceData.Location.Y, PlaceData.Location.Z);

	return SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(VAlign_Center)
		.Padding(0.0f, 0.0f, 10.0f, 0.0f)
		[
			SNew(STextBlock)
				.Text(FText::FromString(PlaceData.PlaceName))
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 10))
		]
		+ SHorizontalBox::Slot()
		.FillWidth(1.0f)
		.VAlign(VAlign_Center)
		[
			SNew(STextBlock)
				.Text(FText::FromString(DisplayCoord))
				.ColorAndOpacity(FSlateColor(FLinearColor(0.5f, 0.5f, 0.5f, 1.0f)))
		]
		+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SButton)
				.Text(FText::FromString(TEXT("  GO  ")))
				.OnClicked(this, &SImplActions::OnGoToPlaceClicked, PlaceData)
		];
}

FReply SImplActions::OnGoToPlaceClicked(FLoadedPlaceData TargetPlace)
{
	FKaliToolsModule& KaliModule = FModuleManager::GetModuleChecked<FKaliToolsModule>("KaliTools");
	KaliModule.TeleportToLocation(TargetPlace.Location, TargetPlace.Rotation);

	return FReply::Handled();
}