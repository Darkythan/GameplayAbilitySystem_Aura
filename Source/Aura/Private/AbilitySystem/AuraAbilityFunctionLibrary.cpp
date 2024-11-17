// Copyright Manel Quintana


#include "AbilitySystem/AuraAbilityFunctionLibrary.h"

#include "AbilitySystemComponent.h"
#include "Game/AuraGameModeBase.h"
#include "Interaction/CombatInterface.h"
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

void UAuraAbilityFunctionLibrary::InitializeDefaultAttributes(ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* AbilitySystemComponent, const UObject* WorldContext)
{
	if (UCharacterClassInfo* ClassInfo = GetCharacterClassInfo(WorldContext))
	{
		const FCharacterClassDefaultInfo ClassDefaultInfo = ClassInfo->GetClassDefaultInfo(CharacterClass);

		const AActor* AvatarActor = AbilitySystemComponent->GetAvatarActor();
		FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
		EffectContextHandle.AddSourceObject(AvatarActor);
		
		const FGameplayEffectSpecHandle PrimaryEffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, EffectContextHandle);
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*PrimaryEffectSpecHandle.Data.Get());
		
		const FGameplayEffectSpecHandle SecondaryEffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(ClassInfo->SecondaryAttributes, Level, EffectContextHandle);
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SecondaryEffectSpecHandle.Data.Get());
		
		const FGameplayEffectSpecHandle VitalEffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(ClassInfo->VitalAttributes, Level, EffectContextHandle);
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*VitalEffectSpecHandle.Data.Get());
	}
}

void UAuraAbilityFunctionLibrary::GiveStartupAbilities(UAbilitySystemComponent* AbilitySystemComponent,
	ECharacterClass CharacterClass, const UObject* WorldContext)
{
	if (UCharacterClassInfo* ClassInfo = GetCharacterClassInfo(WorldContext))
	{
		for (const TSubclassOf<UGameplayAbility> AbilityClass : ClassInfo->CommonAbilities)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1.f);
			AbilitySystemComponent->GiveAbility(AbilitySpec);
		}
		const FCharacterClassDefaultInfo& DefaultInfo = ClassInfo->GetClassDefaultInfo(CharacterClass);
		for (TSubclassOf<UGameplayAbility> AbilityClass : DefaultInfo.StartupAbilities)
		{
			if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(AbilitySystemComponent->GetAvatarActor()))
			{
				FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, CombatInterface->GetLevel());
				AbilitySystemComponent->GiveAbility(AbilitySpec);
			}
		}
	}
}

UCharacterClassInfo* UAuraAbilityFunctionLibrary::GetCharacterClassInfo(const UObject* WorldContext)
{
	if (const AAuraGameModeBase* AuraGameModeBase = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContext)))
	{
		return AuraGameModeBase->CharacterClassInfo;
	}
	return nullptr;
}

bool UAuraAbilityFunctionLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraContext->IsBlockedHit();
	}
	return false;
}

bool UAuraAbilityFunctionLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraContext->IsCriticalHit();
	}
	return false;
}

void UAuraAbilityFunctionLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, const bool bBlocked)
{
	if (FAuraGameplayEffectContext* AuraContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraContext->SetIsBlockedHit(bBlocked);
	}
}

void UAuraAbilityFunctionLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle,
	const bool bCriticalHit)
{
	if (FAuraGameplayEffectContext* AuraContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraContext->SetIsBlockedHit(bCriticalHit);
	}
}

void UAuraAbilityFunctionLibrary::GetLivePlayerWithinRadius(TArray<AActor*>& OutOverlappingActors,
	const TArray<AActor*>& ActorsToIgnore, const float Radius, const FVector& SphereLocation, const UObject* WorldContext)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);
	
	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContext, EGetWorldErrorMode::LogAndReturnNull))
	{
		TArray<FOverlapResult> Overlaps;
		World->OverlapMultiByObjectType(Overlaps, SphereLocation, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);
		for (FOverlapResult& Overlap : Overlaps)
		{
			if (Overlap.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(Overlap.GetActor()))
			{
				OutOverlappingActors.AddUnique(ICombatInterface::Execute_GetAvatar(Overlap.GetActor()));
			}
		}
	}
}

bool UAuraAbilityFunctionLibrary::IsNotFriend(const AActor* FirstActor, const AActor* SecondActor)
{
	const bool bBothArePlayers = FirstActor->ActorHasTag(FName("Player")) && SecondActor->ActorHasTag(FName("Player"));
	const bool bBothAreEnemies = FirstActor->ActorHasTag(FName("Enemy")) && SecondActor->ActorHasTag(FName("Enemy"));
	const bool bFriends = bBothArePlayers || bBothAreEnemies;
	return !bFriends;
}
