// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG/Public/Items/Shields/RPGShieldComponent.h"

URPGShieldComponent::URPGShieldComponent()
{
    SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}
