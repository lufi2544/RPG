// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "RPG/Character/RPGCharacterBase.h"
#include "RPGAttributeSetBase.generated.h"




    #define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
    GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
/**
 * 
 */
UCLASS()
class RPG_API URPGAttributeSetBase : public UAttributeSet
{
	GENERATED_BODY()

	public:

	URPGAttributeSetBase();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData &Data) override;

	//The Character Health Amount
	UPROPERTY(BlueprintReadOnly , Category = "RPG|AttributeSet" , ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(URPGAttributeSetBase, Health);

	//The Character Max Health Amount
	UPROPERTY(BlueprintReadOnly , Category = "RPG|AttributeSet" , ReplicatedUsing = OnRep_MaxHealth)
    FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(URPGAttributeSetBase, MaxHealth);

	//The Amount of Move Speed that the Character has.
	UPROPERTY(BlueprintReadOnly , Category = "RPG|AttributeSet" , ReplicatedUsing = OnRep_MoveSpeed )
	FGameplayAttributeData MoveSpeed;
	ATTRIBUTE_ACCESSORS(URPGAttributeSetBase , MoveSpeed);

	//The Character Level.
	UPROPERTY(BlueprintReadOnly , Category = "RPG|AttributeSet" ,ReplicatedUsing = OnRep_CharacterLevel)
	FGameplayAttributeData CharacterLevel;
	ATTRIBUTE_ACCESSORS(URPGAttributeSetBase , CharacterLevel);

	

protected:


//Functions for the Attributes replication.
	
	UFUNCTION()
	virtual void OnRep_Health();
	
	UFUNCTION()
   virtual void OnRep_MoveSpeed();

	UFUNCTION()
	virtual void OnRep_MaxHealth();

	UFUNCTION()
	virtual void OnRep_CharacterLevel();
	
	
	
	
	
};
