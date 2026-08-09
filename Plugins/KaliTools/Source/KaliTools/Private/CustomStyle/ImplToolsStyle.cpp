#include "CustomStyle/ImplToolsStyle.h"
#include "Interfaces/IPluginManager.h"
#include "Styling/SlateStyleRegistry.h"
FName FImplToolsStyle::StyleSetName = FName("ImplToolsStyle");
TSharedPtr<FSlateStyleSet> FImplToolsStyle::CreatedSlateStyleSet = nullptr;

void FImplToolsStyle::InitializeIcons()
{
	if (!CreatedSlateStyleSet.IsValid())
	{
		CreatedSlateStyleSet = CreateSlateStyleSet();
		// Register the dereferenced style set object
		FSlateStyleRegistry::RegisterSlateStyle(*CreatedSlateStyleSet);
	}
	
}

TSharedRef<FSlateStyleSet> FImplToolsStyle::CreateSlateStyleSet()
{
	TSharedRef<FSlateStyleSet> CustomStyleSet = MakeShareable(new FSlateStyleSet(StyleSetName));

	const FString IconDirectory = IPluginManager::Get().FindPlugin(TEXT("KaliTools"))->GetBaseDir()/"Resources";

	CustomStyleSet->SetContentRoot(IconDirectory);

	const FVector2D Icon16x16(16.f, 16.f);
	CustomStyleSet->Set("ContentBrowser.ImplementetionTools",
		new FSlateImageBrush(IconDirectory / "Icon_ImplementetionTools.png", Icon16x16));


	return CustomStyleSet;
}

void FImplToolsStyle::ShutDown()
{
}

