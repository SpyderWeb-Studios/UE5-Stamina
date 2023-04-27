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



	StaminaDecayDelegate.BindUFunction(this, "StaminaDecay");
	StaminaRegenDelegate.BindUFunction(this, "StaminaRegenerate");

	GetWorld()->GetTimerManager().SetTimer(StaminaDecayHandle, StaminaDecayDelegate, StaminaDecayRate, true);
	GetWorld()->GetTimerManager().SetTimer(StaminaRegenHandle, StaminaRegenDelegate, StaminaRegenRate, true);
	// ...
	
}

void UStaminaComponent::StaminaDecay()
{
	if (UCommonFunctionLibrary::Decay(Stamina, StaminaDecayStep, 0, bIsEnabled)) {
		ToggleStamina(false);
	}
	// UDebugFunctionLibrary::DebugLog("Stamina: " + FString::SanitizeFloat(Stamina), EDebugType::DT_Log, 5.0f);
}

void UStaminaComponent::StaminaRegenerate()
{
	UCommonFunctionLibrary::Regenerate(Stamina, StaminaRegenStep, MaxStamina, !bIsEnabled);
}


void UStaminaComponent::ToggleStamina(bool bEnableStamina)
{
	bIsEnabled = bEnableStamina;

	if (bIsEnabled && Stamina > 0) {
		// UDebugFunctionLibrary::DebugLog("Stamina Enabled", EDebugType::DT_Log, 5.0f);
		if(CharacterMovementComponent) CharacterMovementComponent->MaxWalkSpeed = 900;
	}
	else {
		// UDebugFunctionLibrary::DebugLog("Stamina Disabled", EDebugType::DT_Log, 5.0f);
		if (CharacterMovementComponent) CharacterMovementComponent->MaxWalkSpeed = 400;
	}
}


void UStaminaComponent::SetCharacterMovementReference(UCharacterMovementComponent* MovementComponent)
{
	CharacterMovementComponent = MovementComponent;
}
