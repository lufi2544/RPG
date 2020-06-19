// Fill out your copyright notice in the Description page of Project Settings.

#include "RPG/Public/AbilitySystem/RPGAbilitySystemComponent.h"


void URPGAbilitySystemComponent::ReceiveDamage(URPGAbilitySystemComponent* SourceASC, float InitialDamage, float FinalDamage)
{
    ReceivedDamaged.Broadcast(SourceASC,InitialDamage,FinalDamage);
}
