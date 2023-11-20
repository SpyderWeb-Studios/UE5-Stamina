#pragma once

#include "CoreMinimal.h"
#include "CommonFunctionLibrary.generated.h"

UCLASS()
class STAMINA_API UCommonFunctionLibrary : public UBlueprintFunctionLibrary
{
  UFUNCTION(BlueprintCallable, Category="Common Function Library")
  static bool Regenerate(UPARAM(ref) float & Value, float Delta, float MaxValue, const bool& bCanRegen);

   UFUNCTION(BlueprintCallable, Category="Common Function Library")
  static bool Decay(UPARAM(ref) float& Value, float Delta, float MinValue, const bool& bCanDecay);
}
