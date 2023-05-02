// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/StaminaComponent.h"

// Sets default values for this component's properties
UStaminaComponent::UStaminaComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UStaminaComponent::BeginPlay()
{
	Super::BeginPlay();

	if(GetOwner()->HasAuthority())
	{
		StaminaDecayDelegate.BindUFunction(this, "StaminaDecay");
		StaminaRegenDelegate.BindUFunction(this, "StaminaRegenerate");

		GetWorld()->GetTimerManager().SetTimer(StaminaDecayHandle, StaminaDecayDelegate, StaminaDecayRate, true);
		GetWorld()->GetTimerManager().SetTimer(StaminaRegenHandle, StaminaRegenDelegate, StaminaRegenRate, true);
	}
	// ...
	
}

void UStaminaComponent::Server_ToggleStaminaActive_Implementation(bool bEnableStamina)
{
	ToggleStamina(bEnableStamina);
}

void UStaminaComponent::StaminaDecay()
{
	if(GetOwner()->HasAuthority())
	{
		if (UCommonFunctionLibrary::Decay(Stamina, StaminaDecayStep, 0, bIsEnabled))
		{
			ToggleStamina(false);
		}
		
		UDebugFunctionLibrary::DebugLogWithObjectContext(this, "Stamina Decayed; " + FString::SanitizeFloat(Stamina), EDebugType::DT_Log, 5.0f);
	}
}

void UStaminaComponent::StaminaRegenerate()
{
	if(GetOwner()->HasAuthority())
	{
		UCommonFunctionLibrary::Regenerate(Stamina, StaminaRegenStep, MaxStamina, !bIsEnabled);
		UDebugFunctionLibrary::DebugLogWithObjectContext(this, "Stamina Regenerated; " + FString::SanitizeFloat(Stamina), EDebugType::DT_Log, 5.0f);
	}
}


void UStaminaComponent::ToggleStamina(bool bEnableStamina)
{
	if(GetOwner()->HasAuthority())
	{
		bIsEnabled = bEnableStamina;

		if (bIsEnabled && Stamina > 0) {
			if(CharacterMovementComponent.IsValid()) CharacterMovementComponent.Get()->MaxWalkSpeed = 900;
		}
		else {
			if (CharacterMovementComponent.IsValid()) CharacterMovementComponent.Get()->MaxWalkSpeed = 400;
		}
	}
}


void UStaminaComponent::SetCharacterMovementReference(UCharacterMovementComponent* MovementComponent)
{
	CharacterMovementComponent = MovementComponent;
}

void UStaminaComponent::OnRep_Stamina()
{
	UDebugFunctionLibrary::DebugLogWithObjectContext(this, "Stamina Replicated; " +FString::SanitizeFloat(Stamina), EDebugType::DT_Log, 5.0f);
}

void UStaminaComponent::OnRep_MaxStamina()
{
	UDebugFunctionLibrary::DebugLogWithObjectContext(this, "Max Stamina Replicated; " + FString::SanitizeFloat(MaxStamina), EDebugType::DT_Log, 5.0f);
}

void UStaminaComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UStaminaComponent, Stamina);
	DOREPLIFETIME(UStaminaComponent, MaxStamina);
}
