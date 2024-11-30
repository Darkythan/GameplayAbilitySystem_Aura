// Copyright Manel Quintana


#include "AbilitySystem/AuraAbilityFunctionLibrary.h"

#include "AbilitySystemComponent.h"
#include "Game/AuraGameModeBase.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"

bool UAuraAbilityFunctionLibrary::MakeWidgetControllerParams(const UObject* WorldContextObject,
	FWidgetControllerParams& OutWCParams, AAuraHUD*& OutAuraHUD)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		OutAuraHUD = Cast<AAuraHUD>(PC->GetHUD());
		if (OutAuraHUD)
		{
			AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();

			OutWCParams.AttributeSet = AS;
			OutWCParams.AbilitySystemComponent = ASC;
			OutWCParams.PlayerState = PS;
			OutWCParams.PlayerController = PC;
			return true;
		}
	}
	return false;
}

UOverlayWidgetController* UAuraAbilityFunctionLibrary::GetOverlayWidgetController(const UObject* WorldContext)
{
	FWidgetControllerParams WCParams;
	AAuraHUD* AuraHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContext, WCParams, AuraHUD))
	{
		return AuraHUD->GetOverlayWidgetController(WCParams);
	}
	return nullptr;
}

UMenuWidgetController* UAuraAbilityFunctionLibrary::GetMenuWidgetController(const UObject* WorldContext)
{
	FWidgetControllerParams WCParams;
	AAuraHUD* AuraHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContext, WCParams, AuraHUD))
	{
		return AuraHUD->GetMenuWidgetController(WCParams);
	}
	return nullptr;
}

USpellMenuWidgetController* UAuraAbilityFunctionLibrary::GetSpellMenuWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	AAuraHUD* AuraHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, AuraHUD))
	{
		return AuraHUD->GetSpellMenuWidgetController(WCParams);
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
			if (AbilitySystemComponent->GetAvatarActor()->Implements<UCombatInterface>())
			{
				FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, ICombatInterface::Execute_GetPlayerLevel(AbilitySystemComponent->GetAvatarActor()));
				AbilitySystemComponent->GiveAbility(AbilitySpec);
			}
		}
	}
}

UCharacterClassInfo* UAuraAbilityFunctionLibrary::GetCharacterClassInfo(const UObject* WorldContext)
{
	const AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContext));
	if (AuraGameMode == nullptr) return nullptr;
	return AuraGameMode->CharacterClassInfo;
}

UAbilityInfo* UAuraAbilityFunctionLibrary::GetAbilityInfo(const UObject* WorldContextObject)
{
	const AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (AuraGameMode == nullptr) return nullptr;
	return AuraGameMode->AbilityInfo;
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

int32 UAuraAbilityFunctionLibrary::GetXPRewardForClassAndLevel(const UObject* WorldContextObject,
	ECharacterClass CharacterClass, int32 CharacterLevel)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return 0;

	const FCharacterClassDefaultInfo& Info = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	const float XPReward = Info.XPReward.GetValueAtLevel(CharacterLevel);

	return static_cast<int32>(XPReward);
}
