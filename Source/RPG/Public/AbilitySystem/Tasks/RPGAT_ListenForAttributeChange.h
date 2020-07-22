// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AttributeSet.h"
#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "AbilitySystemComponent.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "RPGAT_ListenForAttributeChange.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAttributeChanged , FGameplayAttribute, Attribute , float , OldValue , float , NewValue );

/**
 * 
 */
UCLASS(BlueprintType , meta = (ExposedAsyncProxy = AsyncTask))
class RPG_API URPGAT_ListenForAttributeChange : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

	public:

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnAttributeChangedDelegate;

	/** This Task will listen for an attribute change inside an Ability System Component */
	UFUNCTION(BlueprintCallable , Category = "RPG|Tasks", meta = (BlueprintInternalUseOnly = "true"))
	static URPGAT_ListenForAttributeChange* ListenForAttributeChange(FGameplayAttribute Attribute , UAbilitySystemComponent* AbilitySystemComponent);

	UFUNCTION(BlueprintCallable , Category = "RPG|Tasks" , meta =(BlueprintInteralUseOnly = "true"))
	static URPGAT_ListenForAttributeChange* ListenForAttributesChange(TArray<FGameplayAttribute>Attributes ,UAbilitySystemComponent* AbilitySystemComponent);
	

	/** We have to call this function if we want the task to end, if we do not call this function
	 * the task will keep listening for ever.
	 */
	UFUNCTION(BlueprintCallable)
	void EndTask();


	protected:
	UPROPERTY()
	UAbilitySystemComponent* ASC;

	FGameplayAttribute AttributeToListenFor;
	TArray<FGameplayAttribute>AttributesToListenFor;

	//Attributes Change. MOstly handled for the UI
	
	void OnAttributeChanged(const FOnAttributeChangeData& Data);
	
	
};
