// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG/Public/AbilitySystem/RPGCooldownEffect.h"

URPGCooldownEffect::URPGCooldownEffect(){}

FGameplayTag URPGCooldownEffect::GetCoolDownTag()
{
    return  CooldownTag;
}
