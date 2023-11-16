// Copyright 2022-2023 Spyderweb Studios Ltd. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "StaminaComponent.h"
#include "Blueprint/UserWidget.h"
#include "StaminaWidget.generated.h"

/**
 * 
 */
UCLASS()
class STAMINA_API UStaminaWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	UFUNCTION(BlueprintNativeEvent, Category="Stamina" )
		void OnStaminaValueChanged(float NewStaminaValue);

	UFUNCTION(BlueprintNativeEvent, Category="Stamina")
		void OnMaxStaminaValueChanged(float NewMaxStaminaValue);
	
	UFUNCTION(BlueprintPure, Category="Stamina")
		UStaminaComponent* GetStaminaComponent() const { return StaminaComponent.Get(); }

	UFUNCTION(BlueprintCallable, Category="Stamina")
		void SetStaminaComponent(UStaminaComponent* AttachedStaminaComponent);

private:
	TObjectPtr<UStaminaComponent> StaminaComponent;
	
};
