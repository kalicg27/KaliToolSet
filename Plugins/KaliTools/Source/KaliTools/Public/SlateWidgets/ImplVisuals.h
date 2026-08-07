#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class SImplVisuals : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SImplVisuals) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	// Button Callbacks for Viewport & Rendering Actions
	FReply OnToggleNaniteClicked();
	FReply OnSetForceLODClicked(int32 LODLevel);
	FReply OnSetViewModeClicked(EViewModeIndex ViewMode);
};