// All rights reserved by Heena Thadani.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "Ability System/DevbladeAbilitySystemComponent.h"
#include "DevbladeAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
class IPawnUIInterface;
/**
 * 
 */
UCLASS()
class DEVBLADE_API UDevbladeAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UDevbladeAttributeSet();

	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data) override;

	UPROPERTY(BlueprintReadOnly, Category="Health")
	FGameplayAttributeData CurrentHealth;
	ATTRIBUTE_ACCESSORS(UDevbladeAttributeSet, CurrentHealth)

	UPROPERTY(BlueprintReadOnly, Category="Health")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UDevbladeAttributeSet, MaxHealth)

	UPROPERTY(BlueprintReadOnly, Category="Rage")
	FGameplayAttributeData CurrentRage;
	ATTRIBUTE_ACCESSORS(UDevbladeAttributeSet, CurrentRage)

	UPROPERTY(BlueprintReadOnly, Category="Rage")
	FGameplayAttributeData MaxRage;
	ATTRIBUTE_ACCESSORS(UDevbladeAttributeSet, MaxRage)

	UPROPERTY(BlueprintReadOnly, Category="Damage")
	FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(UDevbladeAttributeSet, AttackPower)

	UPROPERTY(BlueprintReadOnly, Category="Damage")
	FGameplayAttributeData DefensePower;
	ATTRIBUTE_ACCESSORS(UDevbladeAttributeSet, DefensePower)
	
	UPROPERTY(BlueprintReadOnly, Category="Damage")
	FGameplayAttributeData DamageTaken;
	ATTRIBUTE_ACCESSORS(UDevbladeAttributeSet, DamageTaken)
private:
	TWeakInterfacePtr<IPawnUIInterface> CachedPawnUIInterface;
};
