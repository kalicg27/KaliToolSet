#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/CurveTable.h"
#include "GameplayTagContainer.h"
#include "KaliToolsPresets.generated.h"

// --- Enums ---
UENUM(BlueprintType)
enum class EFortResourceType : uint8
{
    None,
    Wood,
    Stone,
    Metal,
    Custom
};

UENUM(BlueprintType)
enum class EFortBuildingType : uint8
{
    None,
    Prop,
    Wall,
    Floor,
    Corner,
    Deco
};

// --- Struct Replications matching internal BP types ---
USTRUCT(BlueprintType)
struct FFortAttributeInitializationKey
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName AttributeInitCategory;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName AttributeInitSubcategory;
};

// --- Preset Parameter Package ---
USTRUCT(BlueprintType)
struct FKaliBlueprintSetupParams
{
    GENERATED_BODY()

    // 1. Resource Type
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EFortResourceType ResourceType = EFortResourceType::Metal;

    // 2. Preset
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FGameplayTag PhysObjPresetData;

    // 3. Phys
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FGameplayTag PhysObjPhysData;

    // 4. Buoy
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FGameplayTag PhysObjBuoyData;

    // 5. Building Type
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EFortBuildingType BuildingType = EFortBuildingType::Prop;

    // 6. Resource Amount Override (Curve Table Reference)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FCurveTableRowHandle BuildingResourceAmountOverride;

    // 7. Resource Amount Override Row Name
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName BuildingResourceAmountOverrideRowName;

    // 8. Item Attribute Init Category
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ItemAttributeInitCategory;

    // 9. Single Attribute Init Key
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FFortAttributeInitializationKey AttributeInitKeys;

    // 10. Attribute Init Keys Array (Index [0] and Index [1])
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FFortAttributeInitializationKey> AttributeInitKeysArray;

    // 11. Create Phys
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bCreatePhys = true;

    // 12. Destroy Building
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bDestroyBuilding = false;

    FKaliBlueprintSetupParams()
    {
        AttributeInitKeysArray.SetNum(2);
    }
};

struct FKaliPresetDefinition
{
    FString PresetName;
    FKaliBlueprintSetupParams SetupParams;
};

class FKaliToolsPresets
{
public:
    // --- Hardcoded Default Asset Paths ---
    static inline const FSoftObjectPath DefaultCurveTablePath = FSoftObjectPath(TEXT("/Game/Data/Curves/CT_DefaultBuildingResource.CT_DefaultBuildingResource"));
    static inline const FSoftObjectPath DefaultDataTablePath = FSoftObjectPath(TEXT("/Game/Data/Tables/DT_DefaultAttributes.DT_DefaultAttributes"));

    static const TArray<FString>& GetParentClassOptions()
    {
        static const TArray<FString> ParentClasses = {
            TEXT("BuildingProp"),
            TEXT("BuildingWall"),
            TEXT("BuildingFloor"),
            TEXT("BuildingCorner")
        };
        return ParentClasses;
    }

    static const TArray<FKaliPresetDefinition>& GetPresets()
    {
        static const TArray<FKaliPresetDefinition> Presets = {
            { TEXT("Preset 1: Metal Heavy"), {} },
            { TEXT("Preset 2: Wood Light"),  {} },
            // ... Populate presets as needed
        };
        return Presets;
    }
};