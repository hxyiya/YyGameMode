#include "UProjectSpringLibrary.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "UObject/WeakObjectPtr.h"
#include "Containers/Map.h"

static TMap<TWeakObjectPtr<USpringArmComponent>, float> GSpringZoomVelocity;
static TMap<TWeakObjectPtr<USpringArmComponent>, float> GSpringTargetLengths;

static TMap<TWeakObjectPtr<APawn>, FRotator> GCameraTargetRotations;
static TMap<TWeakObjectPtr<APawn>, FRotator> GCameraRotationVelocities;

float UProjectSpringLibrary::UpdateSpringArmLengthByWheelAxis(
    USpringArmComponent *SpringArm,
    float AxisValue,
    float DeltaTime,
    float ChangeSpeed,
    float MinLength,
    float MaxLength,
    bool bInvert,
    float TimeConstant)
{
    if (!SpringArm)
    {
        return 0.f;
    }

    // 初始化或获取目标长度
    float *TargetPtr = GSpringTargetLengths.Find(SpringArm);
    if (!TargetPtr)
    {
        // 初次调用，以当前实际长度作为起始目标
        TargetPtr = &GSpringTargetLengths.Add(SpringArm, SpringArm->TargetArmLength);
    }
    float &TargetLength = *TargetPtr;

    // 根据输入更新目标长度（持久化累积）
    // 注意：ChangeSpeed 建议在 100-500 左右（每秒单位），滚轮脉冲通常只有一帧
    const float Direction = bInvert ? -1.f : 1.f;
    TargetLength += Direction * AxisValue * ChangeSpeed * DeltaTime;
    TargetLength = FMath::Clamp(TargetLength, MinLength, MaxLength);

    // 物理模拟部分
    const float Current = SpringArm->TargetArmLength;
    float &Vel = GSpringZoomVelocity.FindOrAdd(SpringArm);

    // 防止 TimeConstant 过小导致不稳定
    const float SafeTimeConstant = FMath::Max(0.01f, TimeConstant);
    const float k = 1.0f / (SafeTimeConstant * SafeTimeConstant);
    const float c = 2.0f / SafeTimeConstant;

    // x 是位移偏差 (Current - Target)
    const float x = Current - TargetLength;
    const float a = -k * x - c * Vel;

    Vel += a * DeltaTime;
    float Next = Current + Vel * DeltaTime;

    // 虽然物理模拟可能稍微过冲，但最好也做一次 Clamp 防止穿帮
    // 如果希望保留弹性过冲效果，可以去掉这里的 Clamp，只 Clamp TargetLength
    // 这里保留 Clamp 是为了安全性
    Next = FMath::Clamp(Next, MinLength, MaxLength);

    SpringArm->TargetArmLength = Next;
    return Next;
}

void UProjectSpringLibrary::SetCameraRotate(
    APawn* TargetPawn,
    float MouseX,
    float MouseY,
    float DeltaTime,
    float RotationSpeed,
    float TimeConstant,
    float PitchMin,
    float PitchMax)
{
    if (!TargetPawn)
    {
        return;
    }

    AController* Controller = TargetPawn->GetController();
    if (!Controller)
    {
        return;
    }

    // 初始化或获取目标旋转
    FRotator* TargetPtr = GCameraTargetRotations.Find(TargetPawn);
    if (!TargetPtr)
    {
        // 如果没有记录，则以当前控制旋转为起始点
        TargetPtr = &GCameraTargetRotations.Add(TargetPawn, Controller->GetControlRotation());
    }
    FRotator& TargetRot = *TargetPtr;

    // 应用输入
    // MouseX -> Yaw, MouseY -> Pitch
    TargetRot.Yaw += MouseX * RotationSpeed;
    TargetRot.Pitch += MouseY * RotationSpeed;

    // 限制 Pitch
    TargetRot.Pitch = FMath::Clamp(TargetRot.Pitch, PitchMin, PitchMax);
    // 规范化 Yaw (-180 到 180)
    TargetRot.Normalize();

    // 物理模拟
    // 防止 TimeConstant 过小
    const float SafeTimeConstant = FMath::Max(0.001f, TimeConstant);
    const float k = 1.0f / (SafeTimeConstant * SafeTimeConstant);
    const float c = 2.0f / SafeTimeConstant;

    FRotator CurrentRot = Controller->GetControlRotation();
    FRotator& Vel = GCameraRotationVelocities.FindOrAdd(TargetPawn);

    // 计算偏差 (Current - Target)
    // 使用 GetNormalized() 确保处理角度循环
    FRotator Diff = (CurrentRot - TargetRot).GetNormalized();

    // 弹簧阻尼计算 (a = -k * x - c * v)
    // 注意：这里的 Diff 是 (Current - Target)，相当于 x
    float AccYaw = -k * Diff.Yaw - c * Vel.Yaw;
    float AccPitch = -k * Diff.Pitch - c * Vel.Pitch;
    
    Vel.Yaw += AccYaw * DeltaTime;
    Vel.Pitch += AccPitch * DeltaTime;

    FRotator NextRot = CurrentRot + Vel * DeltaTime;
    NextRot.Normalize();

    // 再次限制 NextRot 的 Pitch
    NextRot.Pitch = FMath::Clamp(NextRot.Pitch, PitchMin, PitchMax);

    Controller->SetControlRotation(NextRot);
}
