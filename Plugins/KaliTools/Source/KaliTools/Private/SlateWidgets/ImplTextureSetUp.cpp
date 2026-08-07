#include "SlateWidgets/ImplTextureSetUp.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SScrollBox.h"

void SImplTextureSetUp::Construct(const FArguments& InArgs)
{
	StoredAssets = InArgs._SelectedAssets;

	FSlateFontInfo SectionFont = FCoreStyle::Get().GetFontStyle(FName("EmbossedText"));
	SectionFont.Size = 18;

	ChildSlot
		[
			SNew(SVerticalBox)

				// Sub-Tab Title
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(10.0f)
				[
					SNew(STextBlock)
						.Text(FText::FromString(TEXT("Texxxxtreeee")))
						.Font(SectionFont)
						.ColorAndOpacity(FLinearColor::White)
				]

				// Main Content Area
				+ SVerticalBox::Slot()
				.FillHeight(1.0f)
				.Padding(10.0f)
				[
					SNew(SScrollBox)
						+ SScrollBox::Slot()
						[
							SNew(STextBlock)
								.Text(FText::FromString(TEXT("Place yourtexure heree iu")))
						]
				]
		];
}