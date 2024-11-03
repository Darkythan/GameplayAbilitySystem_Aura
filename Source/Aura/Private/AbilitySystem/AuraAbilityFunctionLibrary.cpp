// Copyright Manel Quintana


#include "AbilitySystem/AuraAbilityFunctionLibrary.h"

#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"

UOverlayWidgetController* UAuraAbilityFunctionLibrary::GetOverlayWidgetController(const UObject* WorldContext)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContext,0);
			PlayerController && WorldContext)
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PlayerController->GetHUD()))
		{
			AAuraPlayerState* PlayerState = PlayerController->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* AbilitySystemComponent = PlayerState->GetAbilitySystemComponent();
			UAttributeSet* AttributeSet = PlayerState->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams (PlayerController, PlayerState, AbilitySystemComponent, AttributeSet);
			return AuraHUD->GetOverlayWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

UMenuWidgetController* UAuraAbilityFunctionLibrary::GetMenuWidgetController(const UObject* WorldContext)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContext,0);
			PlayerController && WorldContext)
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PlayerController->GetHUD()))
		{
			AAuraPlayerState* PlayerState = PlayerController->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* AbilitySystemComponent = PlayerState->GetAbilitySystemComponent();
			UAttributeSet* AttributeSet = PlayerState->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams (PlayerController, PlayerState, AbilitySystemComponent, AttributeSet);
			return AuraHUD->GetMenuWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}
