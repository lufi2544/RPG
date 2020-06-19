// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG/Public/AbilitySystem/RPGAttributeSetBase.h"

#include <xutility>


#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "RPG/Public/Character/RPGPlayerController.h"

//We set the base values on the Player Attribute GameplayEffect
URPGAttributeSetBase::URPGAttributeSetBase()
{
}

void URPGAttributeSetBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSetBase, Health, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSetBase, MaxHealth, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSetBase, MoveSpeed, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSetBase, CharacterLevel, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSetBase, XP, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSetBase, XPBounty, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSetBase, Armor, COND_None, REPNOTIFY_Always);
    
    
}

void URPGAttributeSetBase::OnRep_Health()
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSetBase , Health,COND_None);
}

void URPGAttributeSetBase::OnRep_MaxHealth()
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSetBase , MaxHealth,COND_None);
}
void URPGAttributeSetBase::OnRep_MoveSpeed()
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSetBase , MoveSpeed,COND_None);
}

void URPGAttributeSetBase::OnRep_CharacterLevel()
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSetBase, CharacterLevel,COND_None);
}

void URPGAttributeSetBase::OnRep_XP()
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSetBase , XP, COND_None)
}

void URPGAttributeSetBase::OnRep_XPBounty()
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSetBase,XPBounty,COND_None)
}

void URPGAttributeSetBase::OnRep_Armor()
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSetBase,Armor,COND_None)
}







void URPGAttributeSetBase::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{

   Super::PostGameplayEffectExecute(Data);

    FGameplayEffectContextHandle Context = Data.EffectSpec.GetEffectContext();
    UAbilitySystemComponent* SourceASC = Context.GetOriginalInstigatorAbilitySystemComponent();
    const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();
    FGameplayTagContainer SpecAssetTags;

    Data.EffectSpec.GetAllAssetTags(SpecAssetTags);

    //Get the Target Actor

    AActor* TargetActor = nullptr;
    AController* TargetController = nullptr;
    ARPGCharacterBase* Targethero = nullptr;
   if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
   {
       TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
       TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
       Targethero = Cast<ARPGCharacterBase>(TargetActor);
   }

    
    //Get The Source Actor
    AActor* SourceActor = nullptr;
    AController* SourceActorController = nullptr;
    ARPGCharacterBase* Sourcehero = nullptr;

   if (SourceASC && SourceASC->AbilityActorInfo.IsValid() && SourceASC->AbilityActorInfo->AvatarActor.IsValid())
   {
       SourceActor = SourceASC->AbilityActorInfo->AvatarActor.Get();

       if (SourceASC->AbilityActorInfo->PlayerController.IsValid())
       {

        SourceActorController = SourceASC->AbilityActorInfo->PlayerController.Get();
           
       }

       Sourcehero = Cast<ARPGCharacterBase>(SourceActor);
   }


   if (Data.EvaluatedData.Attribute == GetDamageAttribute())
   {
       // If the Attribute Change, We will adjust the health.

        float LocalDamageDealth = GetDamage();

       SetDamage(0.0f);

      const float FinalHealth = GetHealth()-LocalDamageDealth;

       //We set the new health acording to the Damage done to the character.
       SetHealth(FMath::Clamp(FinalHealth,0.0f, GetMaxHealth()));


       //TODO set the damage number on the UI.
   }else if(Data.EvaluatedData.Attribute == GetHealthAttribute())
   {
       SetHealth(FMath::Clamp(GetHealth(),0.0f,GetMaxHealth()));
   }

    
    
}
