// Copyright 2022-2023 Spyderweb Studios Ltd. All Rights Reserved.


#include "Components/StaminaWidget.h"

#include "Macros.h"

void UStaminaWidget::OnStaminaValueChanged_Implementation(float NewStaminaValue)
{
	UE_LOG(LogStamina, Log, TEXT("%s"), FORMAT_STRING_WITH_NETMODE(this, "Stamina Value Changed [%f]"), NewStaminaValue);
}

void UStaminaWidget::OnMaxStaminaValueChanged_Implementation(float NewMaxStaminaValue)
{
	UE_LOG(LogStamina, Log, TEXT("%s"), FORMAT_STRING_WITH_NETMODE(this, "Max Stamina Value Changed [%f]"), NewMaxStaminaValue);
}

void UStaminaWidget::SetStaminaComponent(UStaminaComponent* AttachedStaminaComponent)
{
	StaminaComponent = AttachedStaminaComponent;

	UE_LOG(LogStamina, Log, TEXT("%s"), FORMAT_STRING_WITH_NETMODE(this, "Attempting to Set Stamina Component"));
	
	if(IsValid(StaminaComponent))
	{
		UE_LOG(LogStamina, Log, TEXT("%s"), FORMAT_STRING_WITH_NETMODE(this, "Stamina Component Set"));
		StaminaComponent->OnStaminaValueUpdated.AddUniqueDynamic(this, &UStaminaWidget::OnStaminaValueChanged);
		StaminaComponent->OnMaxStaminaValueUpdated.AddUniqueDynamic(this, &UStaminaWidget::OnMaxStaminaValueChanged);
 
		OnStaminaValueChanged(StaminaComponent->GetCurrentStamina());
		OnMaxStaminaValueChanged(StaminaComponent->GetCurrentMaxStamina());
	}
}
