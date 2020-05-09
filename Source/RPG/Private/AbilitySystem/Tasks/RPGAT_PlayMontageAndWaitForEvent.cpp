// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG/Public/AbilitySystem/Tasks/RPGAT_PlayMontageAndWaitForEvent.h"

#include <xkeycheck.h>


#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Animation/AnimInstance.h"
#include "RPG/Public/AbilitySystem/RPGAbilitySystemComponent.h"
#include "RPG/Public/AbilitySystem/RPGGameplayAbility.h"
#include "RPG/RPG.h"
#include "GameFramework/Character.h"

URPGAT_PlayMontageAndWaitForEvent::URPGAT_PlayMontageAndWaitForEvent(const FObjectInitializer& ObjectInitializer)
    :Super(ObjectInitializer)
{
    Rate = 1.f;
    bStopWhenAbilityEnds = true;
    
}

URPGAbilitySystemComponent* URPGAT_PlayMontageAndWaitForEvent::GetTargetASC()
{
    return Cast<URPGAbilitySystemComponent>(AbilitySystemComponent);
}

void URPGAT_PlayMontageAndWaitForEvent::OnMontageBlendingOut(UAnimMontage* Montage, bool bInterrupted)
{
    if (Ability && Ability->GetCurrentMontage() == MontageToPlay)
    {
        if (Montage == MontageToPlay)
        {
            AbilitySystemComponent->ClearAnimatingAbility(Ability);

            ACharacter* Character = Cast<ACharacter>(GetAvatarActor());

            if(Character && Character->GetLocalRole() == ROLE_Authority ||
                Character->GetLocalRole() == ROLE_AutonomousProxy && Ability->GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::LocalPredicted)
            {
                Character->SetAnimRootMotionTranslationScale(1.f);
            }
        }
    }

    if(bInterrupted)
    {
        if (ShouldBroadcastAbilityTaskDelegates())
        {
            OnInterrupted.Broadcast(FGameplayTag() ,FGameplayEventData());
        }
    }else
    {
        if (ShouldBroadcastAbilityTaskDelegates())
        {
            OnBlendOut.Broadcast(FGameplayTag() , FGameplayEventData());
        }
    }
}

void URPGAT_PlayMontageAndWaitForEvent::OnAbilityCancelled()
{
    if(StopPlayingMontage())
    {
        if (ShouldBroadcastAbilityTaskDelegates())
        {
            OnCancelled.Broadcast(FGameplayTag() , FGameplayEventData());
        }
    }
}

void URPGAT_PlayMontageAndWaitForEvent::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    if (!bInterrupted)
    {
        if(ShouldBroadcastAbilityTaskDelegates())
        {
            OnCompleted.Broadcast(FGameplayTag() , FGameplayEventData());
        }
    }
    EndTask();
}

void URPGAT_PlayMontageAndWaitForEvent::OnGameplayEvent(FGameplayTag EventTag, const FGameplayEventData* Payload)
{
    if (ShouldBroadcastAbilityTaskDelegates())
    {
        FGameplayEventData TempData = *Payload;
        TempData.EventTag = EventTag;

        EventReceived.Broadcast(EventTag,TempData);
    }
}

URPGAT_PlayMontageAndWaitForEvent* URPGAT_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(
    UGameplayAbility* OwningAbility, FName TaskInstance, UAnimMontage* MontageToPlay, FGameplayTagContainer EventTags,
    float Rate, bool bStopWhenAbilityEnds, FName StartSection, float AnimRootMotionTranslationScale)
{
    UAbilitySystemGlobals::NonShipping_ApplyGlobalAbilityScaler_Rate(Rate);

    URPGAT_PlayMontageAndWaitForEvent* MyObj = NewAbilityTask<URPGAT_PlayMontageAndWaitForEvent>(OwningAbility , TaskInstance);

    MyObj->MontageToPlay = MontageToPlay;
    MyObj->EventTags = EventTags;
    MyObj->Rate = Rate;
    MyObj->StartSection = StartSection;
    MyObj->AnimRootMotionTranslationScale = AnimRootMotionTranslationScale;
    MyObj->bStopWhenAbilityEnds = bStopWhenAbilityEnds;

    return MyObj;
}

void URPGAT_PlayMontageAndWaitForEvent::Activate()
{
    if (Ability == nullptr)
    {return;}

    bool bPlayedMontage = false;
    URPGAbilitySystemComponent* ASC = GetTargetASC();

    if(ASC)
    {
        const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
        UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();
        if (AnimInstance != nullptr)
        {
          EventHandle = ASC->AddGameplayEventTagContainerDelegate(EventTags,FGameplayEventTagMulticastDelegate::FDelegate::CreateUObject(this ,&URPGAT_PlayMontageAndWaitForEvent::OnGameplayEvent));
        }

        if (ASC->PlayMontage(Ability,Ability->GetCurrentActivationInfo() , MontageToPlay,Rate,StartSection) > 0.f)
        {
            if(ShouldBroadcastAbilityTaskDelegates() == false)
            {
                return;
            }


            CancelledHandle = Ability->OnGameplayAbilityCancelled.AddUObject(this, &URPGAT_PlayMontageAndWaitForEvent::OnAbilityCancelled);

            BlendingOutDelegate.BindUObject(this,&URPGAT_PlayMontageAndWaitForEvent::OnMontageBlendingOut);
            AnimInstance->Montage_SetBlendingOutDelegate(BlendingOutDelegate,MontageToPlay);

            MontageEndedDelegate.BindUObject(this , &URPGAT_PlayMontageAndWaitForEvent::OnMontageEnded);
            AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate,MontageToPlay);

            ACharacter* Character = Cast<ACharacter>(GetAvatarActor());
            if (Character && (Character->GetLocalRole() == ROLE_Authority ||
                (Character->GetLocalRole() == ROLE_AutonomousProxy && Ability->GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::LocalPredicted)))
            {
                Character->SetAnimRootMotionTranslationScale(AnimRootMotionTranslationScale);
            }

            bPlayedMontage = true;
        }    else
        {
            UE_LOG(LogTemp, Warning, TEXT("URPGAbilityTask_PlayMontageAndWaitForEvent call to PlayMontage failed!"));
        }
    
    }else
    {
        UE_LOG(LogTemp, Warning, TEXT("URPGAbilityTask_PlayMontageAndWaitForEvent called on an invalid Ability System!"));
    }

    if(!bPlayedMontage)
    {
        UE_LOG(LogTemp, Warning, TEXT("URPGAbilityTask_PlayMontageAndWaitForEvent called in Ability %s failed to play montage %s; Task Instance Name %s."), *Ability->GetName(), *GetNameSafe(MontageToPlay), *InstanceName.ToString());
        if (ShouldBroadcastAbilityTaskDelegates())
        {
            OnCancelled.Broadcast(FGameplayTag() , FGameplayEventData());
        }

        SetWaitingOnAvatar();
    }
        
    
}

void URPGAT_PlayMontageAndWaitForEvent::ExternalCancel()
{
    check(AbilitySystemComponent);

    OnAbilityCancelled();
    Super::ExternalCancel();
}

void URPGAT_PlayMontageAndWaitForEvent::OnDestroy(bool AbilityEnded)
{
    if (Ability)
    {
        Ability->OnGameplayAbilityCancelled.Remove(CancelledHandle);
        if (AbilityEnded && bStopWhenAbilityEnds)
        {
            StopPlayingMontage();
        }
    }

    URPGAbilitySystemComponent* ASC = GetTargetASC();
    if (ASC)
    {
        ASC->RemoveGameplayEventTagContainerDelegate(EventTags , EventHandle);
    }

    Super::OnDestroy(AbilityEnded);
}

bool URPGAT_PlayMontageAndWaitForEvent::StopPlayingMontage()
{
    const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
    if(!ActorInfo)
    {
        return false;
    }

    UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();
    if(AnimInstance == nullptr)
    {
        return false;
    }
    // Check if the montage is still playing
    // The ability would have been interrupted, in which case we should automatically stop the montage
    if(AbilitySystemComponent && Ability)
    {
        if(AbilitySystemComponent->GetAnimatingAbility() == Ability
          && AbilitySystemComponent->GetCurrentMontage() == MontageToPlay)
        {
            FAnimMontageInstance* MontageInstance = AnimInstance->GetActiveInstanceForMontage(MontageToPlay);
            if (MontageInstance)
            {
                MontageInstance->OnMontageBlendingOutStarted.Unbind();
                MontageInstance->OnMontageEnded.Unbind();
            }

            AbilitySystemComponent->CurrentMontageStop();
            return true;
        }
    }

    return false;
}

FString URPGAT_PlayMontageAndWaitForEvent::GetDebugString() const
{
    UAnimMontage* PlayingMontage = nullptr;
    if (Ability)
    {
        const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
        UAnimInstance* AnimInstance = ActorInfo->GetAnimInstance();

        if(AnimInstance != nullptr)
        {
            PlayingMontage = AnimInstance->Montage_IsActive(MontageToPlay) ? MontageToPlay : AnimInstance->GetCurrentActiveMontage();
        }
    }

    return FString::Printf(TEXT("PlayMontageAndWaitForEvent. MontageToPlay: %s  (Currently Playing): %s"), *GetNameSafe(MontageToPlay), *GetNameSafe(PlayingMontage));
}


 
