// Copyright Manel Quintana


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UAuraProjectileSpell::SpawnFireBolt(const FVector TargetLocation) const
{
	if (GetAvatarActorFromActorInfo()->HasAuthority())
	{
		if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo()))
		{
			const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();
			FRotator Rotator = (TargetLocation - SocketLocation).Rotation();
			Rotator.Pitch = 0.f;

			FTransform SpawnTransform;
			SpawnTransform.SetLocation(SocketLocation);
			SpawnTransform.SetRotation(Rotator.Quaternion());
			
			AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(ProjectileClass, SpawnTransform, GetOwningActorFromActorInfo(),
				Cast<APawn>(GetOwningActorFromActorInfo()), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

			const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
			
			FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
			EffectContextHandle.SetAbility(this);
			EffectContextHandle.AddSourceObject(Projectile);

			TArray<TWeakObjectPtr<AActor>> Actors;
			Actors.Add(Projectile);
			EffectContextHandle.AddActors(Actors);

			FHitResult HitResult;
			HitResult.Location = TargetLocation;
			EffectContextHandle.AddHitResult(HitResult);
		
			const FGameplayEffectSpecHandle EffectSpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), EffectContextHandle);

			const FAuraGameplayTags AuraGameplayTags = FAuraGameplayTags::Get();
			const float ScaledDamage = Damage.GetValueAtLevel(GetAbilityLevel());

			UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle,AuraGameplayTags.Damage, ScaledDamage);
			Projectile->DamageEffectSpecHandle = EffectSpecHandle;
			
			if (Projectile)
			{
				Projectile->FinishSpawning(SpawnTransform);
			}
		}
	}
}
