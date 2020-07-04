// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG/Public/AbilitySystem/Targeting/RPGAbilityTargetActor.h"

#include "Kismet/KismetSystemLibrary.h"

void ARPGAbilityTargetActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ARPGAbilityTargetActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
}

void ARPGAbilityTargetActor::SphereTraceWithFilter(TArray<FHitResult>& Out_HitResults,
    const FRPGGameplayTargetDataFilterHandle FilterHandle, const FVector& Start, const FVector& End)
{

    TArray<FHitResult>HitResults;
    
   bool bHit =  UKismetSystemLibrary::SphereTraceMultiForObjects(
    FilterHandle.DataFilter->SelfActor,
    Start,
    End,
    Radius,
    ObjectTypes,
    false,
    IgnoreActors,
    DrawDebug,
    HitResults,
    true
  
    );


    for(FHitResult HitResult : HitResults)
    {
        if (!IsValid(HitResult.GetActor()) || FilterHandle.DataFilter->FilterPassesForActor(HitResult.GetActor()))
        {

            HitResult.bBlockingHit = true;
            Out_HitResults.Add(HitResult);
            
        }
        
    }
    
}

void ARPGAbilityTargetActor::StartTargeting(UGameplayAbility* Ability)
{
    Super::StartTargeting(Ability);

    SourceActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();

    //TODO add reticle logic

    
}

void ARPGAbilityTargetActor::ConfirmTargetingAndContinue()
{
    check(ShouldProduceTargetData());

    TArray<FHitResult>HitResults;

    FVector EndLocation = FVector(StartLocation.GetTargetingTransform().GetLocation().X,StartLocation.GetTargetingTransform().GetLocation().Y,StartLocation.GetTargetingTransform().GetLocation().Z +2.f);

    SphereTraceWithFilter(HitResults,RPGFilter,StartLocation.GetTargetingTransform().GetLocation(),EndLocation);
    
    if (SourceActor)
    {
        FGameplayAbilityTargetDataHandle TargetDataHandle = MakeTargetData(HitResults);

        TargetDataReadyDelegate.Broadcast(TargetDataHandle);
    }

    
    
}

void ARPGAbilityTargetActor::Tick(float DeltaSeconds)
{
}

FGameplayAbilityTargetDataHandle ARPGAbilityTargetActor::MakeTargetData(const TArray<FHitResult>&HitResults) const
{
    FGameplayAbilityTargetDataHandle Out_TargetDataHandle;

    for (FHitResult HitResult : HitResults)
    {

    
        //Pointer and a new object is necessary    
        FGameplayAbilityTargetData* NewData = new FGameplayAbilityTargetData_SingleTargetHit(HitResult);

        Out_TargetDataHandle.Add(NewData);
  
    }

    return Out_TargetDataHandle;
}
