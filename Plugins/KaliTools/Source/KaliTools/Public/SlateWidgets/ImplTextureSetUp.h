#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Input/SComboBox.h"
#include "AssetRegistry/AssetData.h"
#include "ImplToolsSetTypes.h"   
class SImplTextureSetUp : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SImplTextureSetUp) {}
        SLATE_ARGUMENT(TArray<TSharedPtr<FAssetData>>, SelectedAssets)
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);

private:
    TArray<FPropertyRowData> TrackedProperties;
    TArray<TSharedPtr<FAssetData>> SelectedAssetsList;
    TWeakObjectPtr<class UBlueprint> SelectedBlueprint;
    TSharedPtr<class SVerticalBox> DynamicRowsContainer;

    TSharedPtr<STextBlock> StatusTextBlock;
    FText StatusText;

    void RebuildPropertyRowsUI();

    // 👈 Declare missing Slate combo box delegates
    TSharedRef<SWidget> GenerateEnumComboWidget(TSharedPtr<FString> InItem);
    FText GetCurrentEnumText(FName PropName) const;
    void OnEnumSelectionChanged(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo, FName PropName);

    FReply OnFetchPressed();
    FReply OnApplyPressed();
    FReply OnValidatePressed();
};