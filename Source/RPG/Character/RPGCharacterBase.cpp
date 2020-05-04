// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "RPGCharacterMovementComponent.h"
#include "RPG/AbilitySystem/RPGAttributeSetBase.h"
#include "Components/CapsuleComponent.h"
#include "RPG/AbilitySystem/RPGGameplayAbility.h"
#include "RPG/RPGAbilitySystemComponent.h"

// Sets default values
ARPGCharacterBase::ARPGCharacterBase(const class FObjectInitializer& ObjectInitializer):
Super(ObjectInitializer.SetDefaultSubobjectClass<URPGCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility , ECollisionResponse::ECR_Overlap);

	bAlwaysRelevant = true;

}

// Called when the game starts or when spawned
void ARPGCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

UAbilitySystemComponent* ARPGCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

float ARPGCharacterBase::GetHealth() const
{
	if(AttributeSet.IsValid())
	{
		return AttributeSet->GetHealth();
	}
	
	return 0.0f;
}

float ARPGCharacterBase::GetMaxHealth() const
{
	if(AttributeSet.IsValid())
	{
	return AttributeSet->GetMaxHealth();	
	}
	
	return 0.0f;
}

float ARPGCharacterBase::GetMoveSpeed() const
{
	if(AttributeSet.IsValid())
	{
		return AttributeSet->GetMoveSpeed();
	}

	return 0.0f;
}

float ARPGCharacterBase::GetCharacterLevel() const
{
	if (AttributeSet.IsValid())
	{
		AttributeSet->GetCharacterLevel();
	}

	return 0.0f;
}

bool ARPGCharacterBase::IsAlive() const
{
	return GetHealth() > 0;
}


void ARPGCharacterBase::InitializeAttributes()
{

	//Can run on Server and Client
	if(IsValid(CharacterInitialAttributes) && AbilitySystemComponent.IsValid() && AttributeSet.IsValid())
	{

		FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();

		FGameplayEffectSpecHandle EffectSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(CharacterInitialAttributes,GetCharacterLevel(),EffectContextHandle);

		if(EffectSpecHandle.IsValid())
		{

			FActiveGameplayEffectHandle ActiveGameplayEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*EffectSpecHandle.Data.Get(),AbilitySystemComponent.Get());


		}

		
		
	}
}

void ARPGCharacterBase::IniitalizeAbilities()
{

	//For now we are going to let this for later usage.
}

void ARPGCharacterBase::SetHealth(float fNewHealth)
{
	if(AttributeSet.IsValid())
	{
		AttributeSet->SetHealth(fNewHealth);
	}
	
}




