// Copyright Manel Quintana

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AuraAbilityFunctionLibrary.generated.h"

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
};
