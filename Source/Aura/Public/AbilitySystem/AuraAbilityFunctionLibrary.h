// Copyright Manel Quintana

#pragma once

#include "CoreMinimal.h"
#include "AuraAbilityTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Data/CharacterClassInfo.h"
#include "AuraAbilityFunctionLibrary.generated.h"

class UAbilitySystemComponent;
class UMenuWidgetController;
class UOverlayWidgetController;
/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilityFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContext);
	UFUNCTION(BlueprintPure)
	static UMenuWidgetController* GetMenuWidgetController(const UObject* WorldContext);

	UFUNCTION(BlueprintCallable)
	static void InitializeDefaultAttributes(ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* AbilitySystemComponent, const UObject* WorldContext);

	UFUNCTION(BlueprintCallable)
	static void GiveStartupAbilities(UAbilitySystemComponent* AbilitySystemComponent, const UObject* WorldContext);

	UFUNCTION(BlueprintCallable)
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContext);

	UFUNCTION(BlueprintPure)
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintPure)
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable)
	static void SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, const bool bBlocked);

	UFUNCTION(BlueprintCallable)
	static void SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle, const bool bCriticalHit);
};
