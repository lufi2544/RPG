// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystemComponent.h"
#include "RPGGameplayAbility.h"

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
