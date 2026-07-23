// presto3000 Chris Pawlowski All Rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInteractionFocusChanged, AActor*, FocusedActor, FText, InteractionText);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MINIINTERACTIONINVENTORYPLUGIN_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInteractionComponent();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction")
	float TraceDistance = 300.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction")
	float TraceRadius = 20.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interaction")
	float ScanInterval = 0.1f;

	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnInteractionFocusChanged OnFocusChanged;

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void TryInteract();

	UFUNCTION(BlueprintPure, Category = "Interaction")
	AActor* GetFocusedActor() const { return FocusedActor.Get(); }

protected:
	virtual void BeginPlay() override;

private:
	void ScanForInteractable();

	FTimerHandle ScanTimerHandle;

	UPROPERTY()
	TWeakObjectPtr<AActor> FocusedActor;

	float InteractionRadius = 300.f;
};
