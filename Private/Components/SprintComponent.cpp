// Copyright 2022-2023 Spyderweb Studios Ltd. All Rights Reserved.


#include "Components/SprintComponent.h"
#include "Components/StaminaComponent.h"

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
				GetWorld()->GetTimerManager().PauseTimer(StaminaRegenHandle);
				
				CharacterMovementComponent.Get()->MaxWalkSpeed = SprintSpeed;
				GetStaminaComponent()->ToggleStamina(true);
			}
		}
		else {
			if (CharacterMovementComponent.IsValid() && GetStaminaComponent().IsValid())
			{
				GetWorld()->GetTimerManager().PauseTimer(StaminaDecayHandle);
				GetWorld()->GetTimerManager().UnPauseTimer(StaminaRegenHandle);
				
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
		StaminaComponent = GetOwner()->FindComponentByClass<UStaminaComponent>();

		if(StaminaComponent.IsValid())
		{
			
			StaminaComponent->OnStaminaEnabled.AddUniqueDynamic(this, &USprintComponent::StaminaEnabled);
		}

		CharacterMovementComponent = GetOwner()->FindComponentByClass<UCharacterMovementComponent>();

		if(CharacterMovementComponent.IsValid())
		{
			UDebugFunctionLibrary::DebugLogWithObjectContext(this, "Character Movement Component Found", EDebugType::DT_Log, 5.0f);
		}

		StaminaDecayDelegate.BindUObject(this, &USprintComponent::SprintStaminaDecay);
		StaminaRegenDelegate.BindUObject(this, &USprintComponent::SprintStaminaRegenerate);

		GetWorld()->GetTimerManager().SetTimer(StaminaDecayHandle, StaminaDecayDelegate, StaminaDecayRate, true);
		GetWorld()->GetTimerManager().PauseTimer(StaminaDecayHandle);
		
		GetWorld()->GetTimerManager().SetTimer(StaminaRegenHandle, StaminaRegenDelegate, StaminaRegenRate, true);
		GetWorld()->GetTimerManager().PauseTimer(StaminaRegenHandle);
		
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

void USprintComponent::SprintStaminaRegenerate()
{
	if(GetOwner()->HasAuthority() && !bSprintEnabled && GetStaminaComponent().IsValid())
	{
		if(GetStaminaComponent()->RegenerateStamina(StaminaRegenStep))
		{
			UDebugFunctionLibrary::DebugLogWithObjectContext(this, "Sprint Fully Regenerated");
		}
	}
}
