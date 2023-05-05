// Copyright 2022-2023 Spyderweb Studios Ltd. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

#include "Engine/EngineTypes.h"
#include "TimerManager.h"
#include "FunctionLibrary/CommonFunctionLibrary.h"
#include "FunctionLibrary/DebugFunctionLibrary.h"

#include "StaminaComponent.generated.h"

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

protected:
	
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
	
};
