#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

#include "Engine/Blueprint.h"
#include "Selection.h"
#include "Editor.h"
#include "Kismet2/KismetEditorUtilities.h"


struct FPropertyRowData
{
    FName PropertyName;
    FString CurrentValue;
    TSharedPtr<class SEditableTextBox> TextBoxWidget;
};
class SImplTextureSetUp : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SImplTextureSetUp) {}
		SLATE_ARGUMENT(TArray<TSharedPtr<FAssetData>>, SelectedAssets)
	SLATE_END_ARGS()

	/** Constructs this widget */
	void Construct(const FArguments& InArgs);

private:
    TWeakObjectPtr<AActor> SelectedActor;

    // Store rows in an array for dynamic updating
    TArray<FPropertyRowData> TrackedProperties;

    // Container for dynamic Slate children
    TSharedPtr<class SVerticalBox> DynamicRowsContainer;

    FReply OnFetchPressed();
    FReply OnApplyPressed();

    void RebuildPropertyRowsUI();
    TWeakObjectPtr<UBlueprint> SelectedBlueprint;
};