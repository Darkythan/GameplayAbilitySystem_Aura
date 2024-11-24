// Copyright Manel Quintana

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "OverlayWidgetController.generated.h"

class UAbilityInfo;
class UAuraWidget;
struct FOnAttributeChangeData;

USTRUCT(BlueprintType)
struct FUIWidgetRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FGameplayTag MessageTag = FGameplayTag();

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FText Message = FText();

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UAuraWidget> MessageWidget;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	UTexture2D* Image = nullptr;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChangedSignature, int32, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature, FUIWidgetRow, Row);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityInfoSignature, const FAuraAbilityInfo&, Info);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UOverlayWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnManaChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnMaxManaChanged;
	
	virtual void BroadcastInitialValues() override;

	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category="GAS|Messages")
	FMessageWidgetRowSignature MessageWidgetRowDelegate;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Messages")
	FAbilityInfoSignature AbilityInfoDelegate;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|XP")
	FOnAttributeChangedSignature OnXPPercentChangedDelegate;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Level")
	FOnPlayerStatChangedSignature OnPlayerLevelChangedDelegate;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Widget Data")
	TObjectPtr<UDataTable> MessageWidgetDataTable;

	template <typename T>
	T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag);

	void OnInitializeStartupAbilities(UAuraAbilitySystemComponent* AuraAbilitySystemComponent);

	void OnXPChanged(int32 NewXP) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data")
	TObjectPtr<UAbilityInfo> AbilityInfo;
};

template <typename T>
T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag)
{
	return DataTable->FindRow<T>(Tag.GetTagName(), TEXT(""));
}

