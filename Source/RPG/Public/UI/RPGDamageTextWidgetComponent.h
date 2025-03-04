// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "RPGDamageTextWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API URPGDamageTextWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

	public:

		URPGDamageTextWidgetComponent();

		UFUNCTION(BlueprintImplementableEvent , BlueprintCallable)
		void SetDamageText(float Damage);
	
};
