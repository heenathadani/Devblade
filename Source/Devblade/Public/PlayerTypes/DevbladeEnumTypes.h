#pragma once

UENUM()
enum class EDevbladeConfirmType : uint8
{
	Yes,
	No
};

UENUM()
enum class EDevbladeValidType : uint8
{
	Valid,
	Invalid
};

UENUM()
enum class EDevbladeSuccessType : uint8
{
	Successfull,
	Failed
};

UENUM()
enum class EDevbladeCountDownActionInput : uint8
{
	Start,
	Cancel
};

UENUM()
enum class EDevbladeCountDownActionOutput : uint8
{
	Updated,
	Completed,
	Cancelled
};

UENUM(BlueprintType)
enum class EDevbladeGameDifficulty : uint8
{
	Easy,
	Normal,
	Hard,
	VeryHard
};

UENUM(BlueprintType)
enum class EDevbladeInputMode : uint8
{
	GameOnly,
	UIOnly
};