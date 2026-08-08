#pragma once

#include "CoreMinimal.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Input/SComboBox.h"
#include "UObject/Class.h"

// Forward declare Slate types to avoid including heavy Slate headers in this shared header
class SEditableTextBox;
template <typename OptionType> class SComboBox;

enum class EPropertyRowType : uint8
{
    Text,
    Enum
};

struct FPropertyRowData
{
    FName PropertyName;
    FString CurrentValue;
    EPropertyRowType RowType = EPropertyRowType::Text;

    // UI Widgets
    TSharedPtr<SEditableTextBox> TextBoxWidget;
    TSharedPtr<SComboBox<TSharedPtr<FString>>> ComboBoxWidget;

    // Dropdown Data
    TArray<TSharedPtr<FString>> EnumOptions;
    TSharedPtr<FString> SelectedEnumOption;
};

class FImplToolsPropertyUtils
{
public:
    /** Defined inline so no .cpp file is required */
    static inline void AddEnumProperty(TArray<FPropertyRowData>& OutProperties, FName PropName, UEnum* EnumDef, FString DefaultValue = TEXT(""))
    {
        FPropertyRowData Row;
        Row.PropertyName = PropName;
        Row.CurrentValue = DefaultValue;
        Row.RowType = EPropertyRowType::Enum;

        if (EnumDef)
        {
            // NumEnums() - 1 excludes the auto-generated _MAX entry
            for (int32 i = 0; i < EnumDef->NumEnums() - 1; ++i)
            {
                Row.EnumOptions.Add(MakeShared<FString>(EnumDef->GetNameStringByIndex(i)));
            }
        }

        OutProperties.Add(Row);
    }
};