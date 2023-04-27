// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AdrenalineComponent.h"

// Sets default values for this component's properties
UAdrenalineComponent::UAdrenalineComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAdrenalineComponent::BeginPlay()
{
	Super::BeginPlay();



	AdrenalineDecayDelegate.BindUFunction(this, "AdrenalineDecay");
	AdrenalineRegenDelegate.BindUFunction(this, "AdrenalineRegenerate");

	GetWorld()->GetTimerManager().SetTimer(AdrenalineDecayHandle, AdrenalineDecayDelegate, AdrenalineDecayRate, true);
	GetWorld()->GetTimerManager().SetTimer(AdrenalineRegenHandle, AdrenalineRegenDelegate, AdrenalineRegenRate, true);
	// ...

}

void UAdrenalineComponent::AdrenalineDecay()
{
	if (UCommonFunctionLibrary::Decay(Adrenaline, AdrenalineDecayStep, 0, bIsEnabled)) {
		UDebugFunctionLibrary::DebugLog("Adrenaline Disabled: " + FString::SanitizeFloat(Adrenaline), EDebugType::DT_Log, 5.0f);
	}
	// UDebugFunctionLibrary::DebugLog("Adrenaline: " + FString::SanitizeFloat(Adrenaline), EDebugType::DT_Log, 5.0f);
}

void UAdrenalineComponent::AdrenalineRegenerate()
{
	UCommonFunctionLibrary::Regenerate(Adrenaline, AdrenalineRegenStep, MaxAdrenaline, !bIsEnabled);
	// UDebugFunctionLibrary::DebugLog("Adrenaline: " + FString::SanitizeFloat(Adrenaline), EDebugType::DT_Log, 5.0f);
}


