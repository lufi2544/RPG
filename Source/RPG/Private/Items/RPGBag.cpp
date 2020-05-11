// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG/Public/Items/RPGBag.h"

ARPGBag::ARPGBag()
{
    
}

int32 ARPGBag::GetBagCapacity() const
{
    return BagCapacity;
}

int32 ARPGBag::GetActuaBagCapacity() const
{
    return ActuaBagCapacity;
}
