#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/CurveTable.h"
#include "GameplayTagContainer.h"

// ⚠️ IMPORTANT: .generated.h MUST be the last include in this file
#include "ImplPresetPluginData.generated.h"

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

USTRUCT(BlueprintType)
struct FFortAttributeInitializationKey
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Preset")
	FName AttributeInitCategory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Preset")
	FName AttributeInitSubcategory;
};

USTRUCT(BlueprintType)
struct FKaliBlueprintSetupParams
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Preset")
	EFortResourceType ResourceType = EFortResourceType::Metal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Preset")
	FGameplayTag PhysObjPresetData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Preset")
	FGameplayTag PhysObjPhysData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Preset")
	FGameplayTag PhysObjBuoyData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Preset")
	EFortBuildingType BuildingType = EFortBuildingType::Prop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Preset")
	FCurveTableRowHandle BuildingResourceAmountOverride;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Preset")
	FName BuildingResourceAmountOverrideRowName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Preset")
	FName ItemAttributeInitCategory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Preset")
	FFortAttributeInitializationKey AttributeInitKeys;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Preset")
	TArray<FFortAttributeInitializationKey> AttributeInitKeysArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Preset")
	bool bCreatePhys = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Preset")
	bool bDestroyBuilding = false;

	FKaliBlueprintSetupParams()
	{
		AttributeInitKeysArray.SetNum(2);
	}
};

USTRUCT(BlueprintType)
struct FKaliBlueprintSetupData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
	FString ResourceType;
};

class FImplPresetPluginData
{
public:
	static inline const FSoftObjectPath DefaultCurveTablePath = FSoftObjectPath(TEXT("/Game/Data/Curves/CT_DefaultBuildingResource.CT_DefaultBuildingResource"));
	static inline const FSoftObjectPath DefaultDataTablePath = FSoftObjectPath(TEXT("/Game/Data/Tables/DT_DefaultAttributes.DT_DefaultAttributes"));
};