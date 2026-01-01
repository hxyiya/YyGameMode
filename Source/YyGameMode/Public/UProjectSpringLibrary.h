#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "UProjectSpringLibrary.generated.h"

class APawn;
class USpringArmComponent;

UCLASS()
class YYGAMEMODE_API UProjectSpringLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Spring|Control", meta = (DisplayName = "Update SpringArm Length by Wheel Axis", CPP_Default_ChangeSpeed = "100.0", CPP_Default_MinLength = "200.0", CPP_Default_MaxLength = "3000.0", CPP_Default_bInvert = "false", CPP_Default_TimeConstant = "0.15"))
    static float UpdateSpringArmLengthByWheelAxis(
        USpringArmComponent *SpringArm,
        float AxisValue,
        float DeltaTime,
        float ChangeSpeed,
        float MinLength,
        float MaxLength,
        bool bInvert,
        float TimeConstant = 0.15f);

    UFUNCTION(BlueprintCallable, Category = "Spring|Control", meta = (DisplayName = "Set Camera Rotate", CPP_Default_RotationSpeed = "2.0", CPP_Default_TimeConstant = "0.05", CPP_Default_PitchMin = "-80.0", CPP_Default_PitchMax = "80.0"))
    static void SetCameraRotate(
        APawn* TargetPawn,
        float MouseX,
        float MouseY,
        float DeltaTime,
        float RotationSpeed = 2.0f,
        float TimeConstant = 0.05f,
        float PitchMin = -80.0f,
        float PitchMax = 80.0f);
};
