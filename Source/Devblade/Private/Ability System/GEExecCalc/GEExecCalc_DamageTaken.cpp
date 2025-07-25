// All rights reserved by Heena Thadani.

#include "Ability System/GEExecCalc/GEExecCalc_DamageTaken.h"
#include "Ability System/DevbladeAttributeSet.h"
#include "DevbladeGameplayTags.h"
#include "DevbladeDebugHelper.h"

struct FDevbladeDamageCapture
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower)
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePower)
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageTaken)

	FDevbladeDamageCapture()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDevbladeAttributeSet, AttackPower, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDevbladeAttributeSet, DefensePower, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UDevbladeAttributeSet, DamageTaken, Target, false);
	}
};

static const FDevbladeDamageCapture& GetDevbladeDamageCapture()
{
	static FDevbladeDamageCapture DevbladeDamageCapture;
	return DevbladeDamageCapture;
}

UGEExecCalc_DamageTaken::UGEExecCalc_DamageTaken()
{
	//faster way to capture attributes
	RelevantAttributesToCapture.Add(GetDevbladeDamageCapture().AttackPowerDef);
	RelevantAttributesToCapture.Add(GetDevbladeDamageCapture().DefensePowerDef);
	RelevantAttributesToCapture.Add(GetDevbladeDamageCapture().DamageTakenDef);

	//slow way to capture attributes
	/*FProperty* AttackPowerProperty = FindFieldChecked<FProperty>(
		UDevbladeAttributeSet::StaticClass(),
		GET_MEMBER_NAME_CHECKED(UDevbladeAttributeSet, AttackPower)
		);
	FGameplayEffectAttributeCaptureDefinition AttackPowerCaptureDefinition(
	AttackPowerProperty,
	EGameplayEffectAttributeCaptureSource::Source,
	false
	);

	RelevantAttributesToCapture.Add(AttackPowerCaptureDefinition);*/
	
}

void UGEExecCalc_DamageTaken::	Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();

	/*EffectSpec.GetContext().GetSourceObject();
	EffectSpec.GetContext().GetAbility();
	EffectSpec.GetContext().GetInstigator();
	EffectSpec.GetContext().GetEffectCauser();*/
	
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParameters.TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();

	float SourceAttackPower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDevbladeDamageCapture().AttackPowerDef, EvaluateParameters, SourceAttackPower);
	//Debug::Print(TEXT("SourceAttackPower"),SourceAttackPower);
	
	float BaseDamage = 0.f;
	int32 UsedLightAttackComboCount = 0;
	int32 UsedHeavyAttackComboCount = 0;
	for(const TPair<FGameplayTag, float>& TagMagnitude : EffectSpec.SetByCallerTagMagnitudes)
	{
		if(TagMagnitude.Key.MatchesTagExact(DevbladeGameplayTags::Shared_SetByCaller_BaseDamage))
		{
			BaseDamage = TagMagnitude.Value;
		}
		if(TagMagnitude.Key.MatchesTagExact(DevbladeGameplayTags::Player_SetByCaller_AttackType_Light))
		{
			UsedLightAttackComboCount = TagMagnitude.Value;
		}
		if(TagMagnitude.Key.MatchesTagExact(DevbladeGameplayTags::Player_SetByCaller_AttackType_Heavy))
		{
			UsedHeavyAttackComboCount = TagMagnitude.Value;
		}
	}
	float TargetDefensePower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetDevbladeDamageCapture().DefensePowerDef, EvaluateParameters, TargetDefensePower);
	//Debug::Print(TEXT("TargetDefensePower"),TargetDefensePower);
	
	if(UsedLightAttackComboCount != 0)
	{
		const float DamageIncreasePercentLight = (UsedLightAttackComboCount - 1) * 0.05 + 1.f;
		BaseDamage *= DamageIncreasePercentLight;
	}
	if(UsedHeavyAttackComboCount != 0)
	{
		const float DamageIncreasePercentHeavy = (UsedHeavyAttackComboCount) * 0.15 + 1.f;
		BaseDamage *= DamageIncreasePercentHeavy;
	}

	const float FinalDamageDone = BaseDamage * SourceAttackPower / TargetDefensePower;
	//Debug::Print(TEXT("FinalDamageDone"),FinalDamageDone);
	
	if(FinalDamageDone > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(
				GetDevbladeDamageCapture().DamageTakenProperty,
				EGameplayModOp::Override,
				FinalDamageDone
				)
				);
	}
}
