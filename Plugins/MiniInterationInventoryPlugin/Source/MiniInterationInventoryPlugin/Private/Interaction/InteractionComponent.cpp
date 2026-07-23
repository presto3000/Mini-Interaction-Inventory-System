// presto3000 Chris Pawlowski All Rights reserved.


#include "Interaction/InteractionComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"
#include "TimerManager.h"
#include "CollisionShape.h"
#include "Interaction/Interactable.h"
#include "Engine/OverlapResult.h"
#include "Engine/World.h"

UInteractionComponent::UInteractionComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UInteractionComponent::BeginPlay()
{
    Super::BeginPlay();

    GetWorld()->GetTimerManager().SetTimer(
        ScanTimerHandle,
        this,
        &UInteractionComponent::ScanForInteractable,
        ScanInterval,
        true
    );
}

void UInteractionComponent::ScanForInteractable()
{
    AActor* Owner = GetOwner();
    if (!Owner)
    {
        return;
    }
    
    FVector CameraLocation;
    FRotator CameraRotation;
    
    APawn* Pawn = Cast<APawn>(Owner);
    if (!Pawn)
    {
        return;
    }
    
    APlayerController* PC = Cast<APlayerController>(Pawn->GetController());
    if (!PC)
    {
        return;
    }
    
    PC->GetPlayerViewPoint( CameraLocation, CameraRotation);
    FVector CameraForward = CameraRotation.Vector();
    
    // Find everything around player
    TArray<FOverlapResult> Results;

    FCollisionQueryParams Params;
    Params.AddIgnoredActor(Owner);
    
    bool bFound = GetWorld()->OverlapMultiByChannel(Results,Owner->GetActorLocation(),FQuat::Identity,
            ECC_Visibility, FCollisionShape::MakeSphere(InteractionRadius), Params);

    // #if WITH_EDITOR
    // 
    //     DrawDebugSphere(GetWorld(),Owner->GetActorLocation(), InteractionRadius,32,
    //         FColor::Blue,
    //         false,
    //         0.05f
    //     );
    // 
    // #endif
    
    AActor* BestActor = nullptr;
    float BestScore = -999.f;
    
    for (const FOverlapResult& Result : Results)
    {
        AActor* Actor = Result.GetActor();
        if (!Actor)
        {
            continue;
        }

        if (!Actor->Implements<UInteractable>())
        {
            continue;
        }

        if (!IInteractable::Execute_CanInteract(Actor, Owner))
        {
            continue;
        }
        
        FVector ToActor = (Actor->GetActorLocation() - CameraLocation).GetSafeNormal();
        
        // Is object in front of camera?
        float ViewDot = FVector::DotProduct(CameraForward,ToActor);
        
        if (ViewDot < 0.2f)
        {
            continue;
        }
        
        float Distance = FVector::Distance(Owner->GetActorLocation(), Actor->GetActorLocation());
        
        float Score = ViewDot * 100.f - Distance;
        
        if (Score > BestScore)
        {
            BestScore = Score;
            BestActor = Actor;
        }

        // #if WITH_EDITOR
        // 
        //         DrawDebugSphere(GetWorld(),Actor->GetActorLocation(),20.f,12,
        //             FColor::Yellow, false, 0.05f);
        // #endif
    }
    
    if (BestActor != FocusedActor.Get())
    {
        FocusedActor = BestActor;

        FText Text = FText::GetEmpty();
        if (BestActor)
        {
            Text = IInteractable::Execute_GetInteractionText(BestActor, Owner);
        }
        OnFocusChanged.Broadcast(BestActor,Text);
    }
}

void UInteractionComponent::TryInteract()
{
    AActor* Owner = GetOwner();
    AActor* Target = FocusedActor.Get();

    if (!Owner || !Target || !Target->Implements<UInteractable>())
    {
        return;
    }

    if (IInteractable::Execute_CanInteract(Target, Owner))
    {
        IInteractable::Execute_Interact(Target, Owner);
        
        FocusedActor.Reset();
        OnFocusChanged.Broadcast(nullptr, FText::GetEmpty());
    }
}
