// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WeldonBPFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class YYGAMEMODE_API UWeldonBPFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** 
	 * 根据当前时间年月日计算星期几
	 * @return 星期几的字符串 (e.g., "星期一")
	 */
	UFUNCTION(BlueprintCallable, Category = "Weldon|Date")
	static FString GetCurrentWeekDayString();
	
};
