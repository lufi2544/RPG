// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG/Public/AbilitySystem/Targeting/RPGAbilityTargetActor.h"

#include "Kismet/KismetSystemLibrary.h"

void ARPGAbilityTargetActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ARPGAbilityTargetActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    //When the Ability Task Ends, we want the reticle to dissapear.
    
    if (WorldReticles.Num() > 0)
    {
       for (AGameplayAbilityWorldReticle* Reticle : WorldReticles)
       {
           Reticle->Destroy();
       }
    }

    Super::EndPlay(EndPlayReason);

  
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

            // We Have to Make sure that we Add the Hit Result just Once.

            HitResult.bBlockingHit = true;

            if (Out_HitResults.Num() > 0)
            {
                bool bCanBeAdded = true;
                for (FHitResult InHitResult : Out_HitResults)
                {
                    if (HitResult.GetActor() == InHitResult.GetActor() )
                    {
                     bCanBeAdded = false;
                    }
                }
                if (bCanBeAdded)
                {
                    Out_HitResults.Add(HitResult);
                }
            }else
            {
                Out_HitResults.Add(HitResult);
            }       
        }     
    }
    
}

void ARPGAbilityTargetActor::StartTargeting(UGameplayAbility* Ability)
{
    Super::StartTargeting(Ability);

    SourceActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();

    // This is more useful for non instant Targeting actors, so we can start targeting a certain actor.
   

    if (ReticleClass)
    {

        TArray<FHitResult>HitResults;

        FVector EndLocation = FVector(StartLocation.GetTargetingTransform().GetLocation().X,StartLocation.GetTargetingTransform().GetLocation().Y,StartLocation.GetTargetingTransform().GetLocation().Z +2.f);

        SphereTraceWithFilter(HitResults,RPGFilter,StartLocation.GetTargetingTransform().GetLocation(),EndLocation);

        for (FHitResult HitResult : HitResults)
        {
            AActor* LocalActor = HitResult.GetActor();
            TargetedActors.Add(LocalActor);
            
            AGameplayAbilityWorldReticle* LocalReticle = GetWorld()->SpawnActor<AGameplayAbilityWorldReticle>(ReticleClass,LocalActor->GetActorLocation(),LocalActor->GetActorRotation());

            WorldReticles.Add(LocalReticle);

            ReticleMap.Add(LocalReticle,LocalActor);

            if (!ShouldProduceTargetDataOnServer)
            {
                LocalReticle->SetReplicates(true);
            }
        }
         
     }
   
}

void ARPGAbilityTargetActor::ConfirmTargetingAndContinue()
{
    check(ShouldProduceTargetData());

    TArray<FHitResult>HitResults;

    FVector EndLocation = FVector(StartLocation.GetTargetingTransform().GetLocation().X,StartLocation.GetTargetingTransform().GetLocation().Y,StartLocation.GetTargetingTransform().GetLocation().Z +2.f);

    // We Get all the Hit Results that has passed the RPGFilter from the Sphere Trace by reference.
    
    SphereTraceWithFilter(HitResults,RPGFilter,StartLocation.GetTargetingTransform().GetLocation(),EndLocation);

    // We create a GameplayAbilityTargetDataHandle from that HitResults and Broadcast the TargetData ImplementableEvent.
    
    if (SourceActor)
    {
        FGameplayAbilityTargetDataHandle TargetDataHandle = MakeTargetData(HitResults);

        TargetDataReadyDelegate.Broadcast(TargetDataHandle);
    }
 
}

void ARPGAbilityTargetActor::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);


    //We Update the Reticles Location for their actors.
    
    if (WorldReticles.Num() > 0)
    {
        for (int32 Index = 0 ; Index < WorldReticles.Num() ; Index++)
        {
            AActor* LocalActor =  *ReticleMap.Find( WorldReticles[ Index ]);

            // We Locate the Reticle on the Top of the player head. 

            WorldReticles[ Index ]->SetActorLocation(FVector(LocalActor->GetActorLocation().X , GetActorLocation().Y , GetActorLocation().Z + 170.0f));
        }   
    }
    
 
    
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
