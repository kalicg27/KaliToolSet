#include "SlateWidgets/ImplToolsWidget.h"


void SImplToolsTab::Construct(const FArguments& InArgs)
{
	bCanSupportFocus = true;

	FSlateFontInfo TitleTextFont = FCoreStyle::Get().GetFontStyle(FName("EmbossedText"));
	TitleTextFont.Size = 30;
	
	ChildSlot
		[
			SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(STextBlock)
						.Text(FText::FromString(TEXT("Implementetion Tools ")))
						.Font(TitleTextFont)
						.Justification(ETextJustify::Center)
						.ColorAndOpacity(FColor::White)
				]
			+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SButton)
						.Text(FText::FromString(TEXT("Force to LOD0")))
						.OnClicked_Lambda([this]()
							{
								SetForcedLOD(0);

							})

				]
			
		];

}