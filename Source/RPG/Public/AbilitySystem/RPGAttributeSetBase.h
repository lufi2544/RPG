// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "RPG/Public/Character/RPGCharacterBase.h"
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

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	

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

	//The Amount of Armor that the Character has.
	UPROPERTY(BlueprintReadOnly, Category= "RPG|AttributeSet", ReplicatedUsing= OnRep_Armor)
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(URPGAttributeSetBase,Armor)

	UPROPERTY(BlueprintReadOnly, Category= "RPG|AttributeSet", ReplicatedUsing= OnRep_LifeSteal)
	FGameplayAttributeData LifeSteal;
	ATTRIBUTE_ACCESSORS(URPGAttributeSetBase,LifeSteal)

	//This Property is a meta attribute, it will be used by the DamageExecutionCalculation class to calculate the final damage. It will turn into -Health.
	//It is a temporary value that only exists on the Server.Not Replicated
	UPROPERTY(BlueprintReadOnly, Category = "RPG|AttributeSet" , meta= (HideFromLevelInfos))
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(URPGAttributeSetBase, Damage)
	
	//The Character Level.
	UPROPERTY(BlueprintReadOnly , Category = "RPG|AttributeSet" ,ReplicatedUsing = OnRep_CharacterLevel)
	FGameplayAttributeData CharacterLevel;
	ATTRIBUTE_ACCESSORS(URPGAttributeSetBase , CharacterLevel);

	//The Amount of Experience that the Character has.
	UPROPERTY(BlueprintReadOnly, Category = "RPG|AttributeSet", ReplicatedUsing = OnRep_XP)
	FGameplayAttributeData XP;
	ATTRIBUTE_ACCESSORS(URPGAttributeSetBase, XP)

	// The Amount of Experience that is given to the Character´s killer.
	UPROPERTY(BlueprintReadOnly, Category = "RPG|AttributeSetBase",ReplicatedUsing = OnRep_XPBounty)
	FGameplayAttributeData XPBounty;
	ATTRIBUTE_ACCESSORS(URPGAttributeSetBase , XPBounty)


protected:


//Functions for the Attributes replication.
	
	UFUNCTION()
	virtual void OnRep_Health(FGameplayAttributeData& HealthOldvalue);

	UFUNCTION()
    virtual void OnRep_MaxHealth(FGameplayAttributeData& MaxHealthOldvalue);
	
	UFUNCTION()
	virtual void OnRep_LifeSteal(FGameplayAttributeData& LifeStealOldvalue);
	
	UFUNCTION()
   virtual void OnRep_MoveSpeed(FGameplayAttributeData& MoveSpeedOldvalue);

	UFUNCTION()
	virtual void OnRep_Armor(FGameplayAttributeData& ArmorOldvalue);

	UFUNCTION()
	virtual void OnRep_CharacterLevel(FGameplayAttributeData& CharacterLevelOldvalue);

	UFUNCTION()
	virtual void OnRep_XP(FGameplayAttributeData& XPOldvalue);

	UFUNCTION()
	virtual void OnRep_XPBounty(FGameplayAttributeData& XPBountyldvalue);

	
	void AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute , const FGameplayAttributeData& MaxAttribute , float NewMaxValue, const FGameplayAttribute& AffectedAttrivuteProperty);
	
	
	
	
};
