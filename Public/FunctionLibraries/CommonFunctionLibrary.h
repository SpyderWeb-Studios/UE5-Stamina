#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CommonFunctionLibrary.generated.h"

UCLASS()
class STAMINA_API UCommonFunctionLibrary : public UBlueprintFunctionLibrary
{
public:

	UFUNCTION(BlueprintCallable, Category="Common Function Library")
	static bool Regenerate(UPARAM(ref) float& Variable, float regenAmount, float maxAmount, const bool& canRegen);

	UFUNCTION(BlueprintCallable, Category="Common Function Library")
	static bool Decay(UPARAM(ref) float& Variable, float decayAmount, float minAmount, const bool& canDecay);
}
