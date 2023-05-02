// Copyright 2022-2023 Spyderweb Studios Ltd. All Rights Reserved.


#include "Components/StaminaWidget.h"

void UStaminaWidget::OnStaminaValueChanged_Implementation(float NewStaminaValue)
{
	UDebugFunctionLibrary::DebugLogWithObject(this, FString::Printf(TEXT("Stamina Value Changed: %f"), NewStaminaValue), EDebugType::DT_Log);
}

void UStaminaWidget::OnMaxStaminaValueChanged_Implementation(float NewMaxStaminaValue)
{
	UDebugFunctionLibrary::DebugLogWithObject(this, FString::Printf(TEXT("Max Stamina Value Changed: %f"), NewMaxStaminaValue), EDebugType::DT_Log);
}

void UStaminaWidget::SetStaminaComponent(UStaminaComponent* AttachedStaminaComponent)
{
	StaminaComponent = AttachedStaminaComponent;

	UDebugFunctionLibrary::DebugLogWithObjectContext(this, FString::Printf(TEXT("Attempting to Set Stamina Component")), EDebugType::DT_Log);
	
	if(IsValid(StaminaComponent))
	{
		UDebugFunctionLibrary::DebugLogWithObjectContext(this, FString::Printf(TEXT("Stamina Component Set: %s"), *StaminaComponent->GetName()), EDebugType::DT_Log);
		StaminaComponent->OnStaminaValueUpdated.AddUniqueDynamic(this, &UStaminaWidget::OnStaminaValueChanged);
		StaminaComponent->OnMaxStaminaValueUpdated.AddUniqueDynamic(this, &UStaminaWidget::OnMaxStaminaValueChanged);
 
		OnStaminaValueChanged(StaminaComponent->GetCurrentStamina());
		OnMaxStaminaValueChanged(StaminaComponent->GetCurrentMaxStamina());
	}
}
