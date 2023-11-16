// Copyright 2022-2023 Spyderweb Studios Ltd. All Rights Reserved.


#include "Components/SprintComponent.h"
#include "Components/StaminaComponent.h"
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
		const FString Message = "Stamina Enabled: " + FString::FromInt(bArg);
		UDebugFunctionLibrary::DebugLogWithObjectContext(this, Message, EDebugType::DT_Log, 5.0f);

		// Only do something if the stamina component reports that the stamina has ran out
		// or that we can now sprint again
		if(bSprintEnabled && !bArg)
		{
			UDebugFunctionLibrary::DebugLogWithObjectContext(this, "Disabling Sprint");
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
			UDebugFunctionLibrary::DebugLogWithObjectContext(this, "Owner Implements Sprint Component Interface", EDebugType::DT_Log, 5.0f);
			StaminaComponent = ISprintComponentInterface::Execute_GetStaminaComponent(GetOwner());
			CharacterMovementComponent = ISprintComponentInterface::Execute_GetCharacterMovementComponent(GetOwner());
		}
		else
		{
			UDebugFunctionLibrary::DebugLogWithObjectContext(this, "Owner Does Not Implement Sprint Component Interface", EDebugType::DT_Log, 5.0f);
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
			UDebugFunctionLibrary::DebugLogWithObjectContext(this, "Character Movement Component Found", EDebugType::DT_Log, 5.0f);
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
		UDebugFunctionLibrary::DebugLogWithObjectContext(this, "SetRegenVariables called on client");
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

