// Copyright Manel Quintana


#include "Input/AuraInputConfig.h"

const UInputAction* UAuraInputConfig::FindAbilityInputActionByTag(const FGameplayTag& GameplayTag, const bool bLogFound) const
{
	for (const FAuraInputAction& InputAction : AbilityInputAction)
	{
		FGameplayTagContainer GameplayTagContainer;
		GameplayTagContainer.AddTag(InputAction.GameplayTag);
		if (GameplayTag.MatchesAnyExact(GameplayTagContainer) && InputAction.InputAction)
		{
			return InputAction.InputAction;
		}
	}
	if (bLogFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Input with tag [%s] not found in AbilityInputAction"), *GameplayTag.ToString())
	}
	return nullptr;
}
