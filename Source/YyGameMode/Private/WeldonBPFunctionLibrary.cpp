// Fill out your copyright notice in the Description page of Project Settings.


#include "WeldonBPFunctionLibrary.h"
#include "Misc/DateTime.h"

FString UWeldonBPFunctionLibrary::GetCurrentWeekDayString()
{
	FDateTime Now = FDateTime::Now();
	EDayOfWeek Day = Now.GetDayOfWeek();

	switch (Day)
	{
	case EDayOfWeek::Monday:
		return TEXT("星期一");
	case EDayOfWeek::Tuesday:
		return TEXT("星期二");
	case EDayOfWeek::Wednesday:
		return TEXT("星期三");
	case EDayOfWeek::Thursday:
		return TEXT("星期四");
	case EDayOfWeek::Friday:
		return TEXT("星期五");
	case EDayOfWeek::Saturday:
		return TEXT("星期六");
	case EDayOfWeek::Sunday:
		return TEXT("星期日");
	default:
		return TEXT("未知");
	}
}
