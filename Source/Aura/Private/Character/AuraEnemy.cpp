// Copyright Manel Quintana


#include "Character/AuraEnemy.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilityFunctionLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Aura/Aura.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/Widget/AuraWidget.h"

AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
}

void AAuraEnemy::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AAuraEnemy::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}

int32 AAuraEnemy::GetLevel()
{
	return Level;
}

void AAuraEnemy::HitReactTagChanged(FGameplayTag CallbackTag, const int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
}

void AAuraEnemy::Die()
{
	SetLifeSpan(LifeSpan);
	Super::Die();
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	InitAbilityActorInfo();
	
	if (HasAuthority())
	{
		UAuraAbilityFunctionLibrary::GiveStartupAbilities(AbilitySystemComponent, this);	
	}

	if (UAuraWidget* AuraWidget = Cast<UAuraWidget>(HealthBar->GetUserWidgetObject()))
	{
		AuraWidget->SetWidgetController(this);
	}

	const FAuraGameplayTags AuraGameplayTags = FAuraGameplayTags::Get();
	if (const UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChange.Broadcast(Data.NewValue);
			});
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChange.Broadcast(Data.NewValue);
			});
		AbilitySystemComponent->RegisterGameplayTagEvent(AuraGameplayTags.Effects_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(
			this, &AAuraEnemy::HitReactTagChanged	
		);
		
		OnHealthChange.Broadcast(AuraAttributeSet->GetHealth());
		OnMaxHealthChange.Broadcast(AuraAttributeSet->GetMaxHealth());
	}
}

void AAuraEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	if (HasAuthority())
	{
		InitializeDefaultAttributes();		
	}
}

void AAuraEnemy::InitializeDefaultAttributes() const
{
	UAuraAbilityFunctionLibrary::InitializeDefaultAttributes(CharacterClass, Level, AbilitySystemComponent, this);
}
