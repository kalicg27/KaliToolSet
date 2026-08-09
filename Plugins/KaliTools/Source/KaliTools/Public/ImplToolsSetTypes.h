#pragma once

#include "CoreMinimal.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Input/SComboBox.h"
#include "UObject/Class.h"
#include "Engine/DataTable.h"
#include "Engine/CurveTable.h"

// Forward declare Slate types to avoid including heavy Slate headers in this shared header
class SEditableTextBox;
template <typename OptionType> class SComboBox;

enum class EPropertyRowType : uint8
{
    Text,
    Enum,
    DataTableRow,
    CurveTableRow
};

struct FPropertyRowData
{
    FName PropertyName;
    FString CurrentValue;
    EPropertyRowType RowType = EPropertyRowType::Text;
    //curves
    TWeakObjectPtr<UDataTable> TargetDataTable;
    TWeakObjectPtr<UCurveTable> TargetCurveTable;

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

    //curves tables
    static inline void AddDataTableRowProperty(TArray<FPropertyRowData>& OutProperties, FName PropName, UDataTable* DataTable, FString DefaultValue = TEXT(""))
    {
        FPropertyRowData Row;
        Row.PropertyName = PropName;
        Row.CurrentValue = DefaultValue;
        Row.RowType = EPropertyRowType::DataTableRow;
        Row.TargetDataTable = DataTable;

        if (DataTable)
        {
            TArray<FName> RowNames = DataTable->GetRowNames();
            for (const FName& RowName : RowNames)
            {
                Row.EnumOptions.Add(MakeShared<FString>(RowName.ToString()));
            }
        }

        OutProperties.Add(Row);
    }

    /** Extracts all Curve Names from a UCurveTable into a row option list */
    static inline void AddCurveTableRowProperty(TArray<FPropertyRowData>& OutProperties, FName PropName, UCurveTable* CurveTable, FString DefaultValue = TEXT(""))
    {
        FPropertyRowData Row;
        Row.PropertyName = PropName;
        Row.CurrentValue = DefaultValue;
        Row.RowType = EPropertyRowType::CurveTableRow;
        Row.TargetCurveTable = CurveTable;

        if (CurveTable)
        {
            for (const TPair<FName, FRealCurve*>& Pair : CurveTable->GetRowMap())
            {
                Row.EnumOptions.Add(MakeShared<FString>(Pair.Key.ToString()));
            }
        }

        OutProperties.Add(Row);
    }
};