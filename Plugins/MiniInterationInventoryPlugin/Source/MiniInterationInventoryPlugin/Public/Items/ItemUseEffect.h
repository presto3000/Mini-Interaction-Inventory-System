// presto3000 Chris Pawlowski All Rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ItemUseEffect.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, EditInlineNew)
class MINIINTERACTIONINVENTORYPLUGIN_API UItemUseEffect : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent)
	bool OnUse(AActor* User);

	virtual bool OnUse_Implementation(AActor* User)
	{
		return true;
	}
};
