#pragma once

#include "Widgets/SCompoundWidget.h"

class SImplToolsTab : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SImplToolsTab) {}

	SLATE_ARGUMENT(FString, TestString)
	SLATE_END_ARGS()

public:
	void Construct(const FArguments& InArgs);
};