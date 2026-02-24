#pragma once

#include "CoreMinimal.h"
#include "MeshComponentType.generated.h"

UENUM(BlueprintType)
enum class EMeshComponentType : uint8 {
    MCT_NONE                 UMETA(DisplayName = "None"),
    MCT_MESH_STATIC          UMETA(DisplayName = "Static Mesh"),
    MCT_MESH_SKELETAL        UMETA(DisplayName = "Skeletal Mesh"),
};