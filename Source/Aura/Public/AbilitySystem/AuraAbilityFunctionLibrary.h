// Copyright Manel Quintana

#pragma once

#include "CoreMinimal.h"
#include "AuraAbilityTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Data/CharacterClassInfo.h"
#include "AuraAbilityFunctionLibrary.generated.h"

class UAbilityInfo;
class AAuraHUD;
struct FWidgetControllerParams;
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
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static bool MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, AAuraHUD*& OutAuraHUD);
	
	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContext);

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static UMenuWidgetController* GetMenuWidgetController(const UObject* WorldContext);

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static USpellMenuWidgetController* GetSpellMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable)
	static void InitializeDefaultAttributes(ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* AbilitySystemComponent, const UObject* WorldContext);

	UFUNCTION(BlueprintCallable)
	static void GiveStartupAbilities(UAbilitySystemComponent* AbilitySystemComponent, ECharacterClass CharacterClass, const UObject* WorldContext);

	UFUNCTION(BlueprintCallable)
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContext);
	
	UFUNCTION(BlueprintCallable, Category="AuraAbilitySystemLibrary|CharacterClassDefaults")
	static UAbilityInfo* GetAbilityInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure)
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintPure)
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable)
	static void SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, const bool bBlocked);

	UFUNCTION(BlueprintCallable)
	static void SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle, const bool bCriticalHit);

	UFUNCTION(BlueprintCallable)
	static void GetLivePlayerWithinRadius(TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereLocation, const UObject* WorldContext);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayMechanics")
	static bool IsNotFriend(const AActor* FirstActor, const AActor* SecondActor);
	
	static int32 GetXPRewardForClassAndLevel(const UObject* WorldContextObject, ECharacterClass CharacterClass, int32 CharacterLevel);
};
