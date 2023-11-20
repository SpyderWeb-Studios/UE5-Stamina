

#include "FunctionLibrary/CommonFunctionLibrary.h"

bool UCommonFunctionLibrary::Regenerate(UPARAM(ref) float& Variable, float regenAmount, float maxAmount,  const bool& canRegen) 
{
	if (canRegen)
	{
		Variable = FMath::Clamp(Variable + regenAmount, 0, maxAmount);
	}

	if (verbose) {
	}

	return Variable >= maxAmount;

}

bool UCommonFunctionLibrary::Decay(UPARAM(ref) float& Variable, float decayAmount, float minAmount, const bool& canDecay)
{
	if (canDecay)
	{
		Variable = FMath::Clamp(Variable - decayAmount, minAmount, Variable);
	}

	return Variable <= minAmount;
}
