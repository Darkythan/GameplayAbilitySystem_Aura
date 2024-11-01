// Copyright Manel Quintana


#include "UI/Widget/AuraWidget.h"

void UAuraWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
