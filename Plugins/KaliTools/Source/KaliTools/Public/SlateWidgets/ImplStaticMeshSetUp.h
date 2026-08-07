#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

class SImplStaticMeshSetUp : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SImplStaticMeshSetUp) {}
		SLATE_ARGUMENT(TArray<TSharedPtr<FAssetData>>, SelectedAssets)
	SLATE_END_ARGS()

	/** Constructs this widget */
	void Construct(const FArguments& InArgs);

private:
	/** Stored assets passed from the parent tab */
	TArray<TSharedPtr<FAssetData>> StoredAssets;
};