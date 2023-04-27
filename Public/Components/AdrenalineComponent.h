// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/StaminaComponent.h"
#include "AdrenalineComponent.generated.h"

/*
* The Adrenaline Component looks at the state of a given stamina and determines if they can get an increase in speed.
* It works off the principal that if a character is fleeing danger, Adrenaline would be released.
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class STAMINA_API UAdrenalineComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAdrenalineComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
public:

	UFUNCTION(BlueprintCallable, BlueprintPure)
		float GetCurrentAdrenaline() { return Adrenaline; }

protected:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		float Adrenaline;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		float MaxAdrenaline;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		float AdrenalineDecayRate;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		float AdrenalineDecayStep;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		float AdrenalineRegenRate;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		float AdrenalineRegenStep;

	UFUNCTION()
		void AdrenalineDecay();

	UFUNCTION()
		void AdrenalineRegenerate();

private:

	FTimerDelegate AdrenalineDecayDelegate;
	FTimerDelegate AdrenalineRegenDelegate;

	FTimerHandle AdrenalineDecayHandle;
	FTimerHandle AdrenalineRegenHandle;

	bool bIsEnabled = true;

	UStaminaComponent* StaminaComponent;
};
