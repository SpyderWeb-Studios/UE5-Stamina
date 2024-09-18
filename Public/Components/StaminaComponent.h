// Copyright 2022-2023 Spyderweb Studios Ltd. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Net/UnrealNetwork.h"

#include "Engine/EngineTypes.h"
#include "TimerManager.h"

#include "StaminaComponent.generated.h"

class UCharacterMovementComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogStamina, Log, All);

/*
* @brief The Stamina Component is used to manage the Stamina of a Character, and to allow the Character to perform actions based on their Stamina
*/
UCLASS( ClassGroup=(Stamina), meta=(BlueprintSpawnableComponent) )
class STAMINA_API UStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStaminaComponent();

	DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FOnStaminaValueUpdated, UStaminaComponent, OnStaminaValueUpdated, float, NewStaminaValue);
	DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FOnMaxStaminaValueUpdated, UStaminaComponent, OnMaxStaminaValueUpdated, float, NewMaxStaminaValue);
	DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FOnStaminaEnabled, UStaminaComponent, OnStaminaEnabled, bool, bStaminaEnabled);
	
	
	/**
	 * @brief Gets the Current Stamina of the Character
	 * @return The Current Stamina of the Character
	 */
	UFUNCTION(BlueprintPure, Category="Stamina|Base")
		float GetCurrentStamina() const { return Stamina; }

	/**
	 * @brief Gets the Current Max Stamina of the Character
	 * @return The Current Max Stamina of the Character
	 */
	UFUNCTION(BlueprintPure, Category="Stamina|Base")
		float GetCurrentMaxStamina() { return MaxStamina; }

	/**
	 * @brief Calculates if the Character can perform an action based on the Stamina Cost
	 * @param StaminaCost The Amount of Stamina the Action will Cost
	 * @return True if the Character can perform the action, False if they cannot
	 */
	UFUNCTION(BlueprintPure, Category="Stamina|Base")
		bool CanPerformAction(float StaminaCost) const { return (Stamina - StaminaCost) >= 0; }

	UFUNCTION()
		void OnRep_Stamina();

	UFUNCTION()
		void OnRep_MaxStamina();

	UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable, Category="Stamina|Base")
	void ToggleStamina(bool bEnableStamina);

	/**
	 * @brief Regenerates the Characters Stamina
	 * @param DeltaStamina The Amount of Stamina to Regenerate
	 * @return True if the Stamina is at Max Stamina, False if it is not
	 */
	UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable, Category="Stamina|Base")
	bool RegenerateStamina(float DeltaStamina);

	/**
	 * @brief Consumes the Characters Stamina
	 * @param DeltaStamina The Amount of Stamina to Consume
	 * @return True if the Stamina is at 0 Stamina, False if it is not
	 */
	UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable, Category="Stamina|Base")
	bool ConsumeStamina(float DeltaStamina);

	/**
	 * @brief Broadcasts when the Characters Stamina has been updated
	 */
	UPROPERTY(BlueprintAssignable, Category="Stamina|Events")
	FOnStaminaValueUpdated OnStaminaValueUpdated;

	/**
	 * @brief Broadcasts when the Characters Max Stamina has been updated
	 */
	UPROPERTY(BlueprintAssignable, Category="Stamina|Events")
	FOnMaxStaminaValueUpdated OnMaxStaminaValueUpdated;

	/**
	 * @brief Broadcasts when the Characters Stamina has been enabled or disabled
	 */
	UPROPERTY(BlueprintAssignable, Category="Stamina|Events")
	FOnStaminaEnabled OnStaminaEnabled;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void BeginPlay() override;

	UFUNCTION()
	void StaminaRegenerate();

protected:

	
	UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable, Category="Sprint|Base")
	    void SetRegenVariables(const float& RegenRate, const float& RegenStep);
	
	/**
	 * @brief Whether or not the Character is Consuming Stamina
	 */
	UPROPERTY(BlueprintReadOnly, Replicated, Category="Stamina|Base")
      	bool bIsEnabled;

	/**
	 * @brief The Current Stamina of the Character
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, ReplicatedUsing=OnRep_Stamina, Category="Stamina|Base")
		float Stamina = 100;

	/**
	 * @brief The Max Stamina of the Character
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, ReplicatedUsing=OnRep_MaxStamina, Category="Stamina|Base")
		float MaxStamina = 100;
	/**
 * @brief The rate at which the stamina will regenerate when not sprinting
 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Stamina|Base")
	float StaminaRegenRate = 1;

	/**
	 * @brief The amount that the stamina will regenerate by when not sprinting
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Stamina|Base")
	float StaminaRegenStep = 1;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category="Stamina|Base")
		bool CanRegenerateStamina() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category="Stamina|Base")
		bool CanConsumeStamina() const;

	UFUNCTION(BlueprintNativeEvent, Category="Stamina|Base")
		void OnStaminaValueChanged();

	UFUNCTION(BlueprintNativeEvent, Category="Stamina|Base")
		void OnMaxStaminaValueChanged();
private:
	
	FTimerDelegate StaminaRegenDelegate;
	
	FTimerHandle StaminaRegenHandle;
	
};



