#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "AssetRegistry/AssetData.h"

struct FPropertyRowData
{
    FName PropertyName;
    FString CurrentValue;
    TSharedPtr<SEditableTextBox> TextBoxWidget;
};

class SImplTextureSetUp : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SImplTextureSetUp) {}
        // 👈 Add this line so Slate accepts SelectedAssets argument
        SLATE_ARGUMENT(TArray<TSharedPtr<FAssetData>>, SelectedAssets)
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);

private:
    TArray<FPropertyRowData> TrackedProperties;
    TWeakObjectPtr<class UBlueprint> SelectedBlueprint;
    TSharedPtr<class SVerticalBox> DynamicRowsContainer;

    void RebuildPropertyRowsUI();
    FReply OnFetchPressed();
    FReply OnApplyPressed();
};