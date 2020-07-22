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
    DOREPLIFETIME_CONDITION_NOTIFY(URPGAttributeSetBase, LifeSteal, COND_None, REPNOTIFY_Always);
    
    
}

void URPGAttributeSetBase::OnRep_Health(FGameplayAttributeData& HealthOldvalue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSetBase , Health,HealthOldvalue);
}

void URPGAttributeSetBase::OnRep_LifeSteal(FGameplayAttributeData& LifeStealOldvalue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSetBase , LifeSteal ,LifeStealOldvalue);
}

void URPGAttributeSetBase::OnRep_MaxHealth(FGameplayAttributeData& MaxHealthOldvalue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSetBase , MaxHealth,MaxHealthOldvalue);
}
void URPGAttributeSetBase::OnRep_MoveSpeed(FGameplayAttributeData& MoveSpeedOldvalue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSetBase , MoveSpeed,MoveSpeedOldvalue);
}

void URPGAttributeSetBase::OnRep_CharacterLevel(FGameplayAttributeData& CharacterLevelOldvalue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSetBase, CharacterLevel,CharacterLevelOldvalue);
}

void URPGAttributeSetBase::OnRep_XP(FGameplayAttributeData& XPOldvalue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSetBase , XP, XPOldvalue)
}

void URPGAttributeSetBase::OnRep_XPBounty(FGameplayAttributeData& XPBountyldvalue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSetBase,XPBounty,XPBountyldvalue)
}

void URPGAttributeSetBase::OnRep_Armor(FGameplayAttributeData& ArmorOldvalue)
{
    GAMEPLAYATTRIBUTE_REPNOTIFY(URPGAttributeSetBase,Armor,ArmorOldvalue)
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
    ARPGCharacterBase* TargetHero = nullptr;
   if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
   {
       TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
       TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
       TargetHero = Cast<ARPGCharacterBase>(TargetActor);
   }

    
    //Get The Source Actor
    AActor* SourceActor = nullptr;
    AController* SourceActorController = nullptr;
    ARPGCharacterBase* SourceHero = nullptr;

   if (SourceASC && SourceASC->AbilityActorInfo.IsValid() && SourceASC->AbilityActorInfo->AvatarActor.IsValid())
   {
       SourceActor = SourceASC->AbilityActorInfo->AvatarActor.Get();

       if (SourceASC->AbilityActorInfo->PlayerController.IsValid())
       {

        SourceActorController = SourceASC->AbilityActorInfo->PlayerController.Get();
           
       }

       SourceHero = Cast<ARPGCharacterBase>(SourceActor);
   }


   if (Data.EvaluatedData.Attribute == GetDamageAttribute())
   {
       // If the Attribute Change, We will adjust the health.

        float LocalDamageDealth = GetDamage();

       SetDamage(0.0f);

      const float FinalHealth = GetHealth()-LocalDamageDealth;

       //We set the new health acording to the Damage done to the character.
       SetHealth(FMath::Clamp(FinalHealth,0.0f, GetMaxHealth()));


       if (SourceHero != TargetHero)
       {
           ARPGPlayerController* PC = Cast<ARPGPlayerController>(SourceActorController);

           if (PC)
           {
               PC->ShowDamageNumber(LocalDamageDealth,TargetHero);
           }
       }
       
   }else if(Data.EvaluatedData.Attribute == GetHealthAttribute())
   {
       SetHealth(FMath::Clamp(GetHealth(),0.0f,GetMaxHealth()));
   }

    
    
}
