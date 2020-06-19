// Fill out your copyright notice in the Description page of Project Settings.
#include "RPG/Public/AbilitySystem/RPGGameplayAbility.h"
#include "RPG/Public/AbilitySystem/RPGAbilityTypes.h"
#include "RPG/Public/AbilitySystem/RPGAbilitySystemComponent.h"
#include "AbilitySystemComponent.h"


URPGGameplayAbility::URPGGameplayAbility()
{

    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

    ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Dead")));
    ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Debuff.Stun")));
}

void URPGGameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
    Super::OnAvatarSet(ActorInfo, Spec);

    if(ActivateAbilityOnGranted)
    {
        bool ActivatedAbility = ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle,false);
    }
    
    
}



void URPGGameplayAbility::CommitExecute(const FGameplayAbilitySpecHandle Handle,
                                        const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
    Super::CommitExecute(Handle, ActorInfo, ActivationInfo);

       // We set the ability ready to start.
    bHasEnded = false;

    if (bIsComboAbility)
    {
        //Depending on the character montages that he will be play, we set the Max Stacks for the Ability.
        AbilityMaxStacks = CharacterMontagesToPlay.Num();
    }

    
    // We commit the resources and add the Ability Stacks if needed.

    // If the ability Is stackable, that means that we can cast the ability in a certain amount of time and is not the first time we pass through here, we add a stack.
    

    
}

void URPGGameplayAbility::ResetAbilityStacks()
{
    AbilityStacks = 0;
}

void URPGGameplayAbility::ApplyCooldown(const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
    Super::ApplyCooldown(Handle, ActorInfo, ActivationInfo);

    
    
}

FRPGGameplayEffectContainerSpec URPGGameplayAbility::MakeEffectContainerSpec(FGameplayTag ContainerTag, const FGameplayEventData& EventData,
    int32 OverrideGameplayLevel)
{
    //We look into the Container Map for GameplayEffects that matches the Event Tag.
    FRPGGameplayEffectContainer* EffectContainer = EffectContainerMap.Find(ContainerTag);

    if (EffectContainer)
    {
        return MakeContainerSpecFromContainer(*EffectContainer,EventData,OverrideGameplayLevel);
    }

    //If we do not find any EffecConaiter that matches the event tag, then we return a null one.
    return FRPGGameplayEffectContainerSpec();
}

FRPGGameplayEffectContainerSpec URPGGameplayAbility::MakeContainerSpecFromContainer(
    const FRPGGameplayEffectContainer& Container, const FGameplayEventData& EventData, int32 OverrideGameplayLevel)
{
    FRPGGameplayEffectContainerSpec ReturnEffectContainerSpec;
    
    // First we figure out out our Actor info
    ARPGCharacterBase* Instigator = Cast<ARPGCharacterBase>(GetAvatarActorFromActorInfo());
    // We make sure that the Player has and Ability System Component.
    URPGAbilitySystemComponent* InstigatorASC = Cast<URPGAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());

    

    if (InstigatorASC)
    {
        // We are going to check if we have a target type to apply the effects, if so, then we apply that logic.This is optional
        // if we have a missile for example, we are going to add the targets later on.
        if (Container.TargetType.Get())
        {
             TArray<FHitResult>HitResults;
             TArray<AActor*>TargetActors;
             const URPGTargetType* InTargetType = Container.TargetType.GetDefaultObject();
             AActor* AvatarActor = GetAvatarActorFromActorInfo();
            
             //We use the Function of the Target Type class (could beeing executed via BP also)
             ReturnEffectContainerSpec.bHitEnemy =  InTargetType->GetTargets(Instigator,Instigator,EventData,HitResults,TargetActors);
             ReturnEffectContainerSpec.HitActors = TargetActors;
             ReturnEffectContainerSpec.HitActorsHitResults = HitResults;
            
            ReturnEffectContainerSpec.AddTargets(HitResults,TargetActors);
        }
        if(OverrideGameplayLevel == INDEX_NONE)
        {
            OverrideGameplayLevel = OverrideGameplayLevel = this->GetAbilityLevel();
        }
        
        for(const TSubclassOf<UGameplayEffect>& EffectClass : Container.EffectsToApply)
        {
            ReturnEffectContainerSpec.TargetGameplayEffectSpecsHandle.Add(MakeOutgoingGameplayEffectSpec(EffectClass,OverrideGameplayLevel));
        }
    }

    return  ReturnEffectContainerSpec;
    
}

TArray<FActiveGameplayEffectHandle> URPGGameplayAbility::ApplyEffectContainerSpec(const FRPGGameplayEffectContainerSpec& ContainerSpec)
{
    //Out Effects Specs Handle
 TArray<FActiveGameplayEffectHandle> ActiveGameplayEffectHandles;

    for ( const FGameplayEffectSpecHandle& EffectSpecHandle : ContainerSpec.TargetGameplayEffectSpecsHandle)
    {
        ActiveGameplayEffectHandles.Append(K2_ApplyGameplayEffectSpecToTarget(EffectSpecHandle,ContainerSpec.TargetData));
    }

    return ActiveGameplayEffectHandles;
}
