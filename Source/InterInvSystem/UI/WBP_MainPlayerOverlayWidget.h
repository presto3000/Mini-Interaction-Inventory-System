// presto3000 Chris Pawlowski All Rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UI/PlayerOverlayWidget.h"
#include "WBP_MainPlayerOverlayWidget.generated.h"

/**
 * 
 */
UCLASS()
class INTERINVSYSTEM_API UWBP_MainPlayerOverlayWidget : public UPlayerOverlayWidget
{
	GENERATED_BODY()

	virtual void OnSaveClicked() override;
	virtual void OnLoadClicked() override;
};
