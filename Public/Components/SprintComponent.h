// Copyright 2022-2023 Spyderweb Studios Ltd. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StaminaComponent.h"
#include "Components/ActorComponent.h"
#include "SprintComponent.generated.h"


class UCharacterMovementComponent;

/*
 * @brief Component to handle sprinting. This component is responsible for toggling the sprinting state on the server and
 * interfaces with the StaminaComponent to handle the stamina decay and regeneration. This component should be added to the
 * same actor as the CharacterMovementComponent and the StaminaComponent.
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup=(Stamina), meta=(BlueprintSpawnableComponent) )
class STAMINA_API USprintComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USprintComponent();

	/**
	 * @brief RPC to toggle stamina on the Server
	 * @param bEnableSprint If the client is requesting the stamina to be enabled or disabled
	 */
	UFUNCTION(Server, Unreliable, BlueprintCallable, Category="Sprint|Toggle")
		void Server_ToggleSprintActive(bool bEnableSprint);

	/**
	 * @brief Server Only Function to toggle stamina on the Server. Should be used over the RPC equivalent when possible.
	 * @param bEnableSprint If the client is requesting the stamina to be enabled or disabled
	 */
	UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable, Category="Sprint|Toggle")
		void ToggleSprint(bool bEnableSprint);

	/**
	 * @brief Receives a notification from the StaminaComponent when the stamina has ran out
	 * @param bArg If the stamina has ran out or not
	 */
	UFUNCTION()
	void StaminaEnabled(bool bArg);
	
	virtual void BeginPlay() override;

	/**
	 * @brief Get the CharacterMovementComponent that this component is attached to
	 * @return The CharacterMovementComponent that this component is attached to
	 */
	TWeakObjectPtr<UCharacterMovementComponent> GetCharacterMovementComponent() const { return CharacterMovementComponent; }

	/**
	 * @brief Get the StaminaComponent that this component is attached to
	 * @return The StaminaComponent that this component is attached to
	 */
	TWeakObjectPtr<UStaminaComponent> GetStaminaComponent() const { return StaminaComponent; }

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	/**
	 * @brief Decays the stamina by the StaminaDecayStep amount for every StaminaDecayRate seconds
	 */
	UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable, Category="Sprint|Base")
		void SprintStaminaDecay();
	
	UFUNCTION(BlueprintAuthorityOnly, BlueprintCallable, Category="Sprint|Base")
	    void SetDecayVariables(const float& DecayRate, const float& DecayStep);
	
	/**
	 * @brief Gets the sprinting state of the character
	 */
	UPROPERTY(BlueprintReadOnly, Replicated, Category="Sprint|Base")
	bool bSprintEnabled = false;

	/**
	 * @brief The speed that the character will move at when sprinting
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Sprint|Base")
	float SprintSpeed = 900;

	/**
	 * @brief The speed that the character will move at when walking
	 */
	UPROPERTY(BlueprintReadOnly, EditAnywhere,  Category="Sprint|Base")
	float WalkSpeed = 400;

	/**
	 * @brief The rate at which the stamina will decay when sprinting
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Sprint|Decay")
	float StaminaDecayRate = 1;

	/**
	 * @brief The amount that the stamina will decay by when sprinting
	 */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Sprint|Decay")
	float StaminaDecayStep = 1;
	

	/**
	 * @brief A Weak Pointer to the CharacterMovementComponent that this component is attached to
	 */
	TWeakObjectPtr<UCharacterMovementComponent> CharacterMovementComponent;

	/**
	 * @brief A Weak Pointer to the StaminaComponent that this component is attached to
	 */
	TWeakObjectPtr<UStaminaComponent> StaminaComponent;

	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	bool CanDecayStamina() const;
	
private:

	FTimerDelegate StaminaDecayDelegate;

	FTimerHandle StaminaDecayHandle;
	
};
