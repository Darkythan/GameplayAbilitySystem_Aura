// Copyright Manel Quintana


#include "UI/WidgetController/MenuWidgetController.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UMenuWidgetController::BindCallbacksToDependencies()
{
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	for (auto& Pair : AS->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair, AS](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Key, Pair.Value());
			});
	}
}

void UMenuWidgetController::BroadcastInitialValues()
{
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);

	check(AttributeInfo)
	for (auto& Pair : AS->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}
}

void UMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& Tag, const FGameplayAttribute& Attribute) const
{
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Tag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
