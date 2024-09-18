// Copyright 2022-2023 Spyderweb Studios Ltd. All Rights Reserved.


#include "Components/StaminaComponent.h"

#include "Macros.h"
#include "GameFramework/CharacterMovementComponent.h"

DEFINE_LOG_CATEGORY(LogStamina);

// Sets default values for this component's properties
UStaminaComponent::UStaminaComponent()
{
	SetIsReplicatedByDefault(true);
}

void UStaminaComponent::ToggleStamina(bool bEnableStamina)
{
	bIsEnabled = bEnableStamina;
	OnStaminaEnabled.Broadcast(bIsEnabled);
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
	if(GetOwner()->HasAuthority())
	{
		StaminaRegenDelegate.BindUObject(this, &UStaminaComponent::StaminaRegenerate);
		GetWorld()->GetTimerManager().SetTimer(StaminaRegenHandle, StaminaRegenDelegate, StaminaRegenRate, true);
	}
}

void UStaminaComponent::StaminaRegenerate()
{
	if(GetOwner()->HasAuthority() && CanRegenerateStamina())
	{
		UE_LOG(LogStamina, Verbose, TEXT("%s"), FORMAT_STRING_WITH_NETMODE(this, "Stamina Regeneration Triggered"));
		if(RegenerateStamina(StaminaRegenStep))
		{
			UE_LOG(LogStamina, Log, TEXT("%s"), FORMAT_STRING_WITH_NETMODE(this, "Stamina Fully Regenerated"));
		}
	}
}

void UStaminaComponent::SetRegenVariables(const float& RegenRate, const float& RegenStep)
{
	// Make sure we are on the server
	if(!GetOwner()->HasAuthority())
	{
		return;
	}
	
	UE_LOG(LogStamina, Log, TEXT("%s"), FORMAT_STRING_WITH_NETMODE(this, "SetRegenVariables called, RegenRate: %f, RegenStep: %f", RegenRate, RegenStep));
	
	// Clear the Timer
	GetWorld()->GetTimerManager().ClearTimer(StaminaRegenHandle);
	StaminaRegenHandle.Invalidate();

	// Set the new values
	StaminaRegenRate = RegenRate;
	StaminaRegenStep = RegenStep;

	// Restart the Timer
	GetWorld()->GetTimerManager().SetTimer(StaminaRegenHandle, StaminaRegenDelegate, StaminaRegenRate, true);
}

bool UStaminaComponent::RegenerateStamina(float DeltaStamina)
{
	if(GetOwner()->HasAuthority())
	{
		UE_LOG(LogStamina, VeryVerbose, TEXT("%s"), FORMAT_STRING_WITH_NETMODE(this, "Stamina is being Regenerated"));
		
		if (CanRegenerateStamina())
		{
			Stamina = FMath::Clamp(Stamina + DeltaStamina, 0, MaxStamina);
		}
		UE_LOG(LogStamina, Verbose, TEXT("Regenerated to [%f]"), Stamina);
		const bool bResult = Stamina >= MaxStamina;
		
		UE_LOG(LogStamina, Verbose, TEXT("%s"), FORMAT_STRING_WITH_NETMODE(this, "Stamina [%f] Regeneration Result: %d", Stamina, bResult));
		
		OnStaminaValueChanged();
		return bResult;
	}
	return false;
}

bool UStaminaComponent::ConsumeStamina(float DeltaStamina)
{
	if(GetOwner()->HasAuthority() && CanConsumeStamina())
	{
		UE_LOG(LogStamina, Verbose, TEXT("%s"), FORMAT_STRING_WITH_NETMODE(this, "Stamina is being Consumed"));

		if (CanConsumeStamina())
		{
			Stamina = FMath::Clamp(Stamina - DeltaStamina, 0, Stamina);
		}
		const bool bResult = Stamina <= 0;
		
		OnStaminaValueChanged();
		return bResult;
	}
	return false;
}

bool UStaminaComponent::CanRegenerateStamina_Implementation() const
{
	return !bIsEnabled;
}

bool UStaminaComponent::CanConsumeStamina_Implementation() const
{
	return bIsEnabled;
}

void UStaminaComponent::OnRep_Stamina()
{
	OnStaminaValueChanged();
}

void UStaminaComponent::OnRep_MaxStamina()
{
	OnMaxStaminaValueChanged();
}

void UStaminaComponent::OnStaminaValueChanged_Implementation()
{
	UE_LOG(LogStamina, Log, TEXT("%s"), FORMAT_STRING_WITH_NETMODE(this, "Stamina Value Updated [%f]", Stamina));
	OnStaminaValueUpdated.Broadcast(Stamina);
}
void UStaminaComponent::OnMaxStaminaValueChanged_Implementation()
{
	UE_LOG(LogStamina, Log, TEXT("%s"), FORMAT_STRING_WITH_NETMODE(this, "Max Stamina Value Updated [%f]", MaxStamina));
	OnMaxStaminaValueUpdated.Broadcast(MaxStamina);
}
