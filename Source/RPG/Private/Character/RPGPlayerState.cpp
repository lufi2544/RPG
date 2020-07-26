// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/RPGPlayerState.h"
#include "RPG/Public/Items/Weapons/RPGWeapon.h"
#include "RPG/Public/Character/RPGHeroCharacter.h"
#include "AbilitySystem/RPGAbilitySystemComponent.h"
#include "AbilitySystem/RPGAttributeSetBase.h"
#include "Items/RPGBag.h"
#include "Net/UnrealNetwork.h"
#include "UI/RPGFloatingStatusBarWidget.h"

ARPGPlayerState::ARPGPlayerState()
{
    AbilitySystemComponent = CreateDefaultSubobject<URPGAbilitySystemComponent>(TEXT("AbilitySystemComponent"));


    /** We set the Ability System to be replicated and also to be replicated in a way that the server
     will not tells us if anothe proxy has suffered a GE(Mixed Replication mode). **/
    AbilitySystemComponent->SetIsReplicated(true);

    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

    AttributeSet = CreateDefaultSubobject<URPGAttributeSetBase>(TEXT("AttributeSet"));

    // It is a must to update the components at the same frequency than the character.
    NetUpdateFrequency = 100.f;

    CreateInventoryComponent();

}

void ARPGPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME_CONDITION_NOTIFY(ARPGPlayerState, InventoryComponent, COND_None, REPNOTIFY_Always);
    
 
}

UAbilitySystemComponent* ARPGPlayerState::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}

URPGAttributeSetBase* ARPGPlayerState::GetAttributeSetBase() const
{
    return AttributeSet;   
}

void ARPGPlayerState::BeginPlay()
{
    Super::BeginPlay();

   HealthChangeDelegateHandle =  AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this,&ARPGPlayerState::HealthChange);
}

float ARPGPlayerState::GetHealth() const
{
    return AttributeSet->GetHealth();
}

float ARPGPlayerState::GetMaxHealth() const
{
    return AttributeSet->GetMaxHealth();
}

float ARPGPlayerState::GetMoveSpeed() const
{
    return AttributeSet->GetMoveSpeed();
}

float ARPGPlayerState::GetCharacterLevel() const
{
    if(AttributeSet)
    {
        AttributeSet->GetCharacterLevel();
    }

    return 0.0f;
}

ARPGPlayerInventoryComponent* ARPGPlayerState::GetInventoryComponent()
{
  return IsValid(InventoryComponent) ? InventoryComponent : nullptr;
}

bool ARPGPlayerState::IsAlive() const
{
    return GetHealth()>0.0f;
}

ERPGTeam ARPGPlayerState::GetTeam()
{
    return Team;
}

void ARPGPlayerState::SetTeam(ERPGTeam NewTeam)
{
    Team = NewTeam;
}



void ARPGPlayerState::HealthChange(const FOnAttributeChangeData& Data)
{
    float NewhealthValue = Data.NewValue;

    //Update the Health Status Bar

    ARPGHeroCharacter* Hero = Cast<ARPGHeroCharacter>(GetPawn());

    if (Hero)
    {
        URPGFloatingStatusBarWidget* FloatingStatusBarWidget = Hero->GetFloatingStatusBar();

        if (FloatingStatusBarWidget)
        {
            FloatingStatusBarWidget->SetHealthPercentage(NewhealthValue / GetMaxHealth());
        }
    }


    //TODO add the dying logic
    
}

void ARPGPlayerState::CreateInventoryComponent()
{
    if (!this->InventoryComponent)
    {
        InventoryComponent = NewObject<ARPGPlayerInventoryComponent>();
        if (InventoryComponent)
        {
            InventoryComponent->SetReplicates(true);
        }
    }
}

void ARPGPlayerState::OnRep_InventoryComponent()
{
    if (InventoryComponent)
    {
        UE_LOG(LogTemp,Error,TEXT("Exists"));
    }else
    {
        UE_LOG(LogTemp,Error,TEXT("Does note Exists"));
    }
 
}











