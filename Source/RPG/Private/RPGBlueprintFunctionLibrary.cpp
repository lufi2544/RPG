// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG/Public/RPGBlueprintFunctionLibrary.h"


#include "Character/RPGHeroCharacter.h"
#include "RPG/Public/AbilitySystem/RPGAbilitySystemComponent.h"


void URPGBlueprintFunctionLibrary::AddTargetsToGameplayEffectSpec(FRPGGameplayEffectContainerSpec ContainerSpec,
    TArray<FHitResult> HitResults, TArray<AActor*> Actors)
{
    if (HitResults.Num() > 0 || Actors.Num() > 0)
    {
        ContainerSpec.AddTargets(HitResults,Actors);
    }
    
}

TArray<FActiveGameplayEffectHandle> URPGBlueprintFunctionLibrary::ApplyGameplayEffectContainerSpec(
    FRPGGameplayEffectContainerSpec ContainerSpec)
{

    TArray<FActiveGameplayEffectHandle>ActiveGameplayEffectHandles;
    
    if (DoesContainerSpecHasEffects(ContainerSpec))
    {
        if (ContainerSpec.HitActorsHitResults.Num() > 0)
        {
            for (FHitResult TargetHitResult : ContainerSpec.HitActorsHitResults)
            {
                if (TargetHitResult.GetActor())
                {
                    AActor* Target = TargetHitResult.GetActor();
                    ARPGCharacterBase* Herotarget = Cast<ARPGCharacterBase>(Target);
                    URPGAbilitySystemComponent* TargetASC = Cast<URPGAbilitySystemComponent>(Herotarget->GetAbilitySystemComponent());
   
                        for (FGameplayEffectSpecHandle SpecHandle : ContainerSpec.TargetGameplayEffectSpecsHandle)
                        {
                            if (SpecHandle.IsValid() && TargetASC)
                            {
                                
                                ActiveGameplayEffectHandles.Add(SpecHandle.Data->GetContext().GetInstigatorAbilitySystemComponent()->BP_ApplyGameplayEffectSpecToTarget(SpecHandle,TargetASC));
                            }
                        }
                    
                    
                }
                
            }
        }else if (ContainerSpec.HitActors.Num() > 0)
        {
            for(AActor* Target : ContainerSpec.HitActors)
            {
                if (Target)
                {
                   ARPGHeroCharacter* HeroTarget = Cast<ARPGHeroCharacter>(Target);
                   URPGAbilitySystemComponent* TargetASC =Cast<URPGAbilitySystemComponent>( HeroTarget->GetAbilitySystemComponent() );

                    for (FGameplayEffectSpecHandle SpecHandle : ContainerSpec.TargetGameplayEffectSpecsHandle)
                    {
                        if (TargetASC)
                        {
                            ActiveGameplayEffectHandles.Add(SpecHandle.Data->GetEffectContext().GetInstigatorAbilitySystemComponent()->BP_ApplyGameplayEffectSpecToTarget(SpecHandle,TargetASC)); 
                        }   
                            
                        
                    }
                }
            }
        }
    }

   return ActiveGameplayEffectHandles;
}

 bool URPGBlueprintFunctionLibrary::DoesContainerSpecHasEffects(FRPGGameplayEffectContainerSpec ContainerSpec)
{

 

    return  ContainerSpec.HasValidEffects();
}
