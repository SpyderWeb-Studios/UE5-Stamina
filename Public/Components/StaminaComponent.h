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
	/**
	 * @brief RPC to toggle stamina on the Server
	 * @param bEnableStamina If the client is requesting the stamina to be enabled or disabled
	 */
	UFUNCTION(Server, Unreliable, BlueprintCallable, Category="Stamina|Toggle")
		void Server_ToggleStaminaActive(bool bEnableStamina);

	/**
	 * @brief Server Only Function to toggle stamina on the Server. Should be used over the RPC equivalent when possible.
	 * @param bEnableStamina If the client is requesting the stamina to be enabled or disabled
	 */
	UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable, Category="Stamina|Toggle")
		void ToggleStamina(bool bEnableStamina);
	

	UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable, Category="Stamina|Base")
		void SetCharacterMovementReference(UCharacterMovementComponent* MovementComponent);

	UFUNCTION(BlueprintPure, Category="Stamina|Base")
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

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Stamina|Decay")
		float StaminaDecayRate = 1;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Stamina|Decay")
		float StaminaDecayStep = 1;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Stamina|Regenerate")
		float StaminaRegenRate = 1;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Stamina|Regenerate")
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
