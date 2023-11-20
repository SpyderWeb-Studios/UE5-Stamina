// Copyright 2022-2023 Spyderweb Studios Ltd. All Rights Reserved.


#include "Components/StaminaComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "FunctionLibraries/CommonFunctionLibrary.h"

// Sets default values for this component's properties
UStaminaComponent::UStaminaComponent()
{
	SetIsReplicatedByDefault(true);
}

void UStaminaComponent::OnRep_Stamina()
{
	OnStaminaValueUpdated.Broadcast(Stamina);
}

void UStaminaComponent::OnRep_MaxStamina()
{
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

void UStaminaComponent::BeginPlay()
{
	Super::BeginPlay();
	if(GetOwner()->HasAuthority()){
		StaminaRegenDelegate.BindUObject(this, &UStaminaComponent::StaminaRegenerate);
			
		GetWorld()->GetTimerManager().SetTimer(StaminaRegenHandle, StaminaRegenDelegate, StaminaRegenRate, true);
	}
}

void UStaminaComponent::StaminaRegenerate()
{
	if(GetOwner()->HasAuthority() && !bIsEnabled)
	{
		if(RegenerateStamina(StaminaRegenStep))
		{
			UE_LOG(LogTemp, Log, TEXT("Stamina Fully Replenished"));
		}
	}
}

void UStaminaComponent::SetRegenVariables(const float& DecayRate, const float& DecayStep)
{
	// Make sure we are on the server
	if(!GetOwner()->HasAuthority())
	{
		return;
	}
	
	// Clear the Timer
	GetWorld()->GetTimerManager().ClearTimer(StaminaRegenHandle);
	StaminaRegenHandle.Invalidate();

	// Set the new values
	StaminaRegenRate = DecayRate;
	StaminaRegenStep = DecayStep;

	// Restart the Timer
	GetWorld()->GetTimerManager().SetTimer(StaminaRegenHandle, StaminaRegenDelegate, StaminaRegenRate, true);
}
