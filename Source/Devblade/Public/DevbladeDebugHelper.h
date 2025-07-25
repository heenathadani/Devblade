﻿#pragma once
namespace Debug
{
	// Helper method to print debug message on screen & log
	// @param Msg: Message to print
	// @param Color: Color of the message
	// @param InKey: Key to identify the message
	static void Print(const FString& Msg, const FColor& Color = FColor::MakeRandomColor(), int32 InKey = -1)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(InKey, 5.f, Color, Msg);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *Msg);
		}
	}
	static void Print(const FString& FloatTitle, float FloatValueToPrint, int32 InKey = -1, const FColor& Color = FColor::MakeRandomColor())
	{
		if (GEngine)
		{
			const FString FinalMsg = FloatTitle + TEXT(": ") + FString::SanitizeFloat(FloatValueToPrint);

			GEngine->AddOnScreenDebugMessage(InKey,7.f,Color,FinalMsg);

			UE_LOG(LogTemp,Warning,TEXT("%s"),*FinalMsg);
		}
	}
}
