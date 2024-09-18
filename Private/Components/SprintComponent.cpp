// Copyright 2022-2023 Spyderweb Studios Ltd. All Rights Reserved.


#include "Components/SprintComponent.h"


#include "Macros.h"
#include "Components/StaminaComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interfaces/SprintComponentInterface.h"

// Sets default values for this component's properties
USprintComponent::USprintComponent()
{
	SetIsReplicatedByDefault(true);
}

void USprintComponent::Server_ToggleSprintActive_Implementation(bool bEnableSprint)
{
	ToggleSprint(bEnableSprint);
}

void USprintComponent::ToggleSprint(bool bEnableSprint)
{
	if(GetOwner()->HasAuthority())
	{
		bSprintEnabled = bEnableSprint;

		if (bSprintEnabled && GetStaminaComponent().IsValid() && GetStaminaComponent()->GetCurrentStamina() > 0) {
			if(CharacterMovementComponent.IsValid())
			{
				GetWorld()->GetTimerManager().UnPauseTimer(StaminaDecayHandle);
				
				CharacterMovementComponent.Get()->MaxWalkSpeed = SprintSpeed;
				GetStaminaComponent()->ToggleStamina(true);
			}
		}
		else {
			if (CharacterMovementComponent.IsValid() && GetStaminaComponent().IsValid())
			{
				GetWorld()->GetTimerManager().PauseTimer(StaminaDecayHandle);
				
				CharacterMovementComponent.Get()->MaxWalkSpeed = WalkSpeed;
				GetStaminaComponent()->ToggleStamina(false);
			}
		}
	}
}

void USprintComponent::StaminaEnabled(bool bArg)
{
	if(GetOwner()->HasAuthority())
	{
		UE_LOG(LogStamina, Log, TEXT("%s"), FORMAT_STRING_WITH_NETMODE(this, "Stamina Enabled: [%d]"), bArg);

		// Only do something if the stamina component reports that the stamina has ran out
		// or that we can now sprint again
		if(bSprintEnabled && !bArg)
		{
			UE_LOG(LogStamina, Log, TEXT("%s"), FORMAT_STRING_WITH_NETMODE(this, "Disabling Sprint"));
			ToggleSprint(false);
		}
	}
}

// Called when the game starts
void USprintComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if(GetOwner()->HasAuthority())
	{

		if(GetOwner()->Implements<USprintComponentInterface>())
		{
			UE_LOG(LogStamina, Log, TEXT("%s"), FORMAT_STRING_WITH_NETMODE(this, "Owner Implements Sprint Component Interface"));
			StaminaComponent = ISprintComponentInterface::Execute_GetStaminaComponent(GetOwner());
			CharacterMovementComponent = ISprintComponentInterface::Execute_GetCharacterMovementComponent(GetOwner());
		}
		else
		{
			UE_LOG(LogStamina, Log, TEXT("%s"), FORMAT_STRING_WITH_NETMODE(this, "Owner Does Not Implements Sprint Component Interface"));
			StaminaComponent = GetOwner()->FindComponentByClass<UStaminaComponent>();
		}
		
		if(StaminaComponent.IsValid())
		{
			StaminaComponent->OnStaminaEnabled.AddUniqueDynamic(this, &USprintComponent::StaminaEnabled);
		}

		if(!CharacterMovementComponent.IsValid())
		{
			CharacterMovementComponent = GetOwner()->FindComponentByClass<UCharacterMovementComponent>();
		}

		
		if(CharacterMovementComponent.IsValid())
		{
			UE_LOG(LogStamina, Log, TEXT("%s"), FORMAT_STRING_WITH_NETMODE(this, "Character Movement Component Found"));
		}
		StaminaDecayDelegate.BindUObject(this, &USprintComponent::SprintStaminaDecay);

		SetDecayVariables(StaminaDecayRate, StaminaDecayStep);
		GetWorld()->GetTimerManager().PauseTimer(StaminaDecayHandle);
	}
	
}

void USprintComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USprintComponent, bSprintEnabled);
}

void USprintComponent::SprintStaminaDecay()
{
	// If the Owner has authority and sprint is enabled and the stamina component is valid
	// Decay the stamina
	if(GetOwner()->HasAuthority() && bSprintEnabled && GetStaminaComponent().IsValid())
	{
		if(GetStaminaComponent()->ConsumeStamina(StaminaDecayStep))
		{
			ToggleSprint(false);
		}
	}
}

void USprintComponent::SetDecayVariables(const float& DecayRate, const float& DecayStep)
{
	// Make sure we are on the server
	if(!GetOwner()->HasAuthority())
	{
		UE_LOG(LogStamina, Error, TEXT("%s"), FORMAT_STRING_WITH_NETMODE(this, "SetRegenVariables called on client"));
		return;
	}
	
	// Clear the Timer
	GetWorld()->GetTimerManager().ClearTimer(StaminaDecayHandle);
	StaminaDecayHandle.Invalidate();

	// Set the new values
	StaminaDecayRate = DecayRate;
	StaminaDecayStep = DecayStep;

	// Restart the Timer
	GetWorld()->GetTimerManager().SetTimer(StaminaDecayHandle, StaminaDecayDelegate, StaminaDecayRate, true);
}

