// Fill out your copyright notice in the Description page of Project Settings.
#include "RPG/Public/AbilitySystem/RPGGameplayAbility.h"
#include "RPG/Public/AbilitySystem/RPGAbilityTypes.h"
#include "RPG/Public/AbilitySystem/RPGAbilitySystemComponent.h"
#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"


URPGGameplayAbility::URPGGameplayAbility()
{

    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

    ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Dead")));
    ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("State.Debuff.Stun")));
}

bool URPGGameplayAbility::CommitAbilityStacks()
{
    bool bSuccess = false;
    
    if (AbilityMaxStacks > 0)
    {
        if (AbilityStacks < AbilityMaxStacks)
        {
            bSuccess = true;
        }    
    }

    return bSuccess;
}

void URPGGameplayAbility::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(URPGGameplayAbility , AbilityStacks);
    DOREPLIFETIME(URPGGameplayAbility , AbilityMaxStacks);
    
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

    
    
}void URPGGameplayAbility::ApplyDamageModifiers( FRPGGameplayEffectContainerSpec& ContainerSpec,FRPGGameplayEffectContainerSpec InContainerSpec)
{
    ContainerSpec = InContainerSpec;
    
    // We basically create a new Spec Handle and add that to the SpecHandles that will be applied later from the Container Spec.
    AActor* Player = GetAvatarActorFromActorInfo();

    if (Player)
    {
        FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(AbilityDamageClass,1.0f,GetAbilitySystemComponentFromActorInfo()->MakeEffectContext());

        SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")),AbilityDamage);

        ContainerSpec.TargetGameplayEffectSpecsHandle.Add(SpecHandle);
    }


}

void URPGGameplayAbility::ApplyRPGCoolDown()
{
    AActor* Player = GetActorInfo().AvatarActor.Get();

    if (Player)
    {
        FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(AbilityCooldownClass,1.0f,GetAbilitySystemComponentFromActorInfo()->MakeEffectContext());

        if (SpecHandle.IsValid())
        {
            SpecHandle.Data->SetSetByCallerMagnitude(AbilityCooldownClass.GetDefaultObject()->GetCoolDownTag(),AbilityCooldown);

            GetAbilitySystemComponentFromActorInfo()->BP_ApplyGameplayEffectSpecToSelf(SpecHandle);
        }
    }
 
}




FRPGGameplayEffectContainerSpec URPGGameplayAbility::MakeEffectContainerSpec(FGameplayTag ContainerTag, const FGameplayEventData& EventData, int32 OverrideGameplayLevel)
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

FRPGGameplayEffectContainerSpec URPGGameplayAbility::MakeContainerSpecFromContainer( const FRPGGameplayEffectContainer& Container, const FGameplayEventData& EventData, int32 OverrideGameplayLevel )
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

             ReturnEffectContainerSpec.AddTargets(HitResults,TargetActors);
            
             ReturnEffectContainerSpec.HitActors = TargetActors;
             ReturnEffectContainerSpec.HitActorsHitResults = HitResults;
            
            
        }
        if(OverrideGameplayLevel == INDEX_NONE)
        {
            OverrideGameplayLevel = OverrideGameplayLevel = this->GetAbilityLevel();
        }
        
        for(const TSubclassOf<UGameplayEffect>& EffectClass : Container.EffectsToApply)
        {
            FGameplayEffectSpecHandle Handle ;
            
            Handle = MakeOutgoingGameplayEffectSpec(EffectClass,OverrideGameplayLevel);

            
            ReturnEffectContainerSpec.TargetGameplayEffectSpecsHandle.Add(Handle);
            
        }
    }

    UE_LOG(LogTemp,Error,TEXT("%f"),ReturnEffectContainerSpec.TargetGameplayEffectSpecsHandle.Num());
    
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
