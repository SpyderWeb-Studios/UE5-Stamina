// Copyright 2022-2023 Spyderweb Studios Ltd. All Rights Reserved.


#include "Components/StaminaComponent.h"

// Sets default values for this component's properties
UStaminaComponent::UStaminaComponent()
{
	SetIsReplicatedByDefault(true);
}

void UStaminaComponent::OnRep_Stamina()
{
	UDebugFunctionLibrary::DebugLogWithObjectContext(this, "Stamina Replicated; " +FString::SanitizeFloat(Stamina), EDebugType::DT_Log, 5.0f);

	OnStaminaValueUpdated.Broadcast(Stamina);
}

void UStaminaComponent::OnRep_MaxStamina()
{
	UDebugFunctionLibrary::DebugLogWithObjectContext(this, "Max Stamina Replicated; " + FString::SanitizeFloat(MaxStamina), EDebugType::DT_Log, 5.0f);

	OnMaxStaminaValueUpdated.Broadcast(MaxStamina);
}

void UStaminaComponent::ToggleStamina(bool bEnableStamina)
{
	bIsEnabled = bEnableStamina;
	OnStaminaEnabled.Broadcast(bIsEnabled);
}

bool UStaminaComponent::RegenerateStamina(float DeltaStamina)
{
	if(GetOwner()->HasAuthority())
	{
		const bool bResult = UCommonFunctionLibrary::Regenerate(Stamina, DeltaStamina, MaxStamina, !bIsEnabled);
		OnRep_Stamina();
		return bResult;
	}
	return false;
}

bool UStaminaComponent::ConsumeStamina(float DeltaStamina)
{
	if(GetOwner()->HasAuthority())
	{
		const bool bResult = UCommonFunctionLibrary::Decay(Stamina, DeltaStamina, 0, bIsEnabled);
		OnRep_Stamina();
		return bResult;
	}
	return false;
}

void UStaminaComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UStaminaComponent, Stamina);
	DOREPLIFETIME(UStaminaComponent, MaxStamina);
	DOREPLIFETIME(UStaminaComponent, bIsEnabled);
}
