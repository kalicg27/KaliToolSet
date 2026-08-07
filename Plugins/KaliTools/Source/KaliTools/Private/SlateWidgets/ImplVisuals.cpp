#include "SlateWidgets/ImplVisuals.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SBox.h"
#include "LevelEditor.h"
#include "SLevelViewport.h"
#include "EditorViewportClient.h"
#include "HAL/IConsoleManager.h"
#include "Modules/ModuleManager.h"

void SImplVisuals::Construct(const FArguments& InArgs)
{
	FSlateFontInfo HeaderFont = FCoreStyle::Get().GetFontStyle(FName("EmbossedText"));
	HeaderFont.Size = 16;

	ChildSlot
	[
		SNew(SVerticalBox)

		// Title Header
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(10.0f)
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Viewport Visual Tools")))
			.Font(HeaderFont)
			.ColorAndOpacity(FLinearColor::White)
		]

		// View Mode Buttons
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(5.0f)
		[
			SNew(SHorizontalBox)

			+ SHorizontalBox::Slot().Padding(2.0f)
			[
				SNew(SButton)
				.Text(FText::FromString(TEXT("Lit Mode")))
				.OnClicked(this, &SImplVisuals::OnSetViewModeClicked, VMI_Lit)
			]

			+ SHorizontalBox::Slot().Padding(2.0f)
			[
				SNew(SButton)
				.Text(FText::FromString(TEXT("Wireframe")))
				.OnClicked(this, &SImplVisuals::OnSetViewModeClicked, VMI_Wireframe)
			]
		]

		// Force LOD Buttons
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(5.0f)
		[
			SNew(SHorizontalBox)

			+ SHorizontalBox::Slot().Padding(2.0f)
			[
				SNew(SButton)
				.Text(FText::FromString(TEXT("Force LOD 0")))
				.OnClicked(this, &SImplVisuals::OnSetForceLODClicked, 0)
			]

			+ SHorizontalBox::Slot().Padding(2.0f)
			[
				SNew(SButton)
				.Text(FText::FromString(TEXT("Reset LOD (Auto)")))
				.OnClicked(this, &SImplVisuals::OnSetForceLODClicked, -1)
			]
		]
	];
}

FReply SImplVisuals::OnSetViewModeClicked(EViewModeIndex ViewMode)
{
	if (GEditor)
	{
		FLevelEditorModule& LevelEditorModule = FModuleManager::GetModuleChecked<FLevelEditorModule>("LevelEditor");
		TSharedPtr<ILevelEditor> LevelEditor = LevelEditorModule.GetFirstLevelEditor();

		if (LevelEditor.IsValid())
		{
			TSharedPtr<SLevelViewport> ActiveViewport = LevelEditor->GetActiveViewportInterface();
			if (ActiveViewport.IsValid())
			{
				// ✅ FIX: Use GetLevelViewportClient() instead of GetLevelEditorViewportClient()
				FLevelEditorViewportClient& ViewportClient = ActiveViewport->GetLevelViewportClient();
				ViewportClient.SetViewMode(ViewMode);
			}
		}
	}
	return FReply::Handled();
}

FReply SImplVisuals::OnSetForceLODClicked(int32 LODLevel)
{
	static IConsoleVariable* CVarForceLOD = IConsoleManager::Get().FindConsoleVariable(TEXT("r.ForceLOD"));
	if (CVarForceLOD)
	{
		CVarForceLOD->Set(LODLevel, ECVF_SetByCode);
	}
	return FReply::Handled();
}

FReply SImplVisuals::OnToggleNaniteClicked()
{
	static IConsoleVariable* CVarNanite = IConsoleManager::Get().FindConsoleVariable(TEXT("r.Nanite"));
	if (CVarNanite)
	{
		int32 CurrentState = CVarNanite->GetInt();
		CVarNanite->Set(CurrentState == 0 ? 1 : 0, ECVF_SetByCode);
	}
	return FReply::Handled();
}