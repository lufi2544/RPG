// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG/Public/Items/Weapons/RPGSwordComponent.h"

URPGSwordComponent::URPGSwordComponent()
{
    SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}
