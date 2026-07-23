// presto3000 Chris Pawlowski All Rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ToggleTarget.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UToggleTarget : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MINIINTERACTIONINVENTORYPLUGIN_API IToggleTarget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, Category = "Toggle")
	void SetToggleState(bool bNewState);
};