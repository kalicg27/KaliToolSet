#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "KaliTools.h"
struct FQuickTeleportInputItem
{
	TSharedPtr<SEditableTextBox> CoordinateInput;
	FString DefaultName;
};


class SImplActions : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SImplActions) {}
		
	SLATE_END_ARGS()

	/** Constructs this widget */
	void Construct(const FArguments& InArgs);

private:

	TArray<TSharedPtr<FQuickTeleportInputItem>> InputItems;
	TSharedPtr<SVerticalBox> InputListContainer;

	// Dynamic containers for Category 3 Loaded List
	TArray<FLoadedPlaceData> LoadedPlaces;
	TSharedPtr<SVerticalBox> LoadedListContainer;

	// Slate Handlers - Category 1
	FReply OnAddPlaceInputClicked();
	FReply OnRemovePlaceInputClicked(TSharedPtr<FQuickTeleportInputItem> ItemToRemove);
	FReply OnLoadPlacesClicked();

	// Slate Handlers - Category 3
	FReply OnGoToPlaceClicked(FLoadedPlaceData TargetPlace);

	// Render Helpers
	void RefreshInputList();
	TSharedRef<SWidget> BuildInputRow(TSharedPtr<FQuickTeleportInputItem> Item, int32 Index);
	void RefreshLoadedList();
	TSharedRef<SWidget> BuildLoadedRow(const FLoadedPlaceData& PlaceData);

};