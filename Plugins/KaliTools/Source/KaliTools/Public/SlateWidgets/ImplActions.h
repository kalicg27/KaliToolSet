#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "KaliTools.h"

// Data structure representing a dynamic input row in Category 1
struct FQuickTeleportInputItem
{
	TSharedPtr<SEditableTextBox> CoordinateInput;
	FString DefaultName;
};

class SImplActions : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SImplActions)
		: _SelectedAssets()
		{
		}
		SLATE_ARGUMENT(TArray<TSharedPtr<FAssetData>>, SelectedAssets)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	// Asset array passed via Slate args
	TArray<TSharedPtr<FAssetData>> StoredAssets;

	// Dynamic Category 1 Inputs
	TArray<TSharedPtr<FQuickTeleportInputItem>> InputItems;
	TSharedPtr<SVerticalBox> InputListContainer;

	// Dynamic Category 3 Loaded List
	TArray<FLoadedPlaceData> LoadedPlaces;
	TSharedPtr<SVerticalBox> LoadedListContainer;

	// Event Handlers
	FReply OnAddPlaceInputClicked();
	FReply OnRemovePlaceInputClicked(TSharedPtr<FQuickTeleportInputItem> ItemToRemove);
	FReply OnLoadPlacesClicked();
	FReply OnGoToPlaceClicked(FLoadedPlaceData TargetPlace);

	// Helpers
	void RefreshInputList();
	TSharedRef<SWidget> BuildInputRow(TSharedPtr<FQuickTeleportInputItem> Item, int32 Index);
	void RefreshLoadedList();
	TSharedRef<SWidget> BuildLoadedRow(const FLoadedPlaceData& PlaceData);
};