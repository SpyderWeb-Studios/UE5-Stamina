// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInput/Public/InputActionValue.h"
#include "Net/UnrealNetwork.h"

#include "FunctionLibrary/CommonFunctionLibrary.h"
#include "FunctionLibrary/DebugFunctionLibrary.h"



#include "InputActionValue.h"
#include "StaminaComponent.generated.h"

/*
* Stamina Component interfaces with a Character Movement Component to provide an easy method of sprinting. Handles it's own Timers to 
* control when the Character can Sprint and when they have ran out of stamina and must come to a halt
* 
*/
UCLASS( ClassGroup=(Stamina), meta=(BlueprintSpawnableComponent) )
class STAMINA_API UStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStaminaComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame

	UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable)
		void ToggleStamina(bool bEnableStamina);
	

	UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable)
		void SetCharacterMovementReference(UCharacterMovementComponent* MovementComponent);

	UFUNCTION(BlueprintCallable, BlueprintPure)
		float GetCurrentStamina() { return Stamina; }

	UFUNCTION()
		void OnRep_Stamina();

	UFUNCTION()
		void OnRep_MaxStamina();


	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, ReplicatedUsing=OnRep_Stamina, Category="Stamina|Base")
		float Stamina = 100;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, ReplicatedUsing=OnRep_MaxStamina, Category="Stamina|Base")
		float MaxStamina = 100;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		float StaminaDecayRate = 1;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		float StaminaDecayStep = 1;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		float StaminaRegenRate = 1;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
		float StaminaRegenStep = 1;

	UFUNCTION()
		void StaminaDecay();

	UFUNCTION()
		void StaminaRegenerate();

private:

	FTimerDelegate StaminaDecayDelegate;
	FTimerDelegate StaminaRegenDelegate;

	FTimerHandle StaminaDecayHandle;
	FTimerHandle StaminaRegenHandle;

	bool bIsEnabled;

	TWeakObjectPtr<UCharacterMovementComponent> CharacterMovementComponent;

	// UCharacterMovementComponent* CharacterMovementComponent;
};
