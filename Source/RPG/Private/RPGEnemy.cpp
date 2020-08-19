// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGEnemy.h"
#include "RPG/Public/AbilitySystem/RPGAbilitySystemComponent.h"
#include "RPG/Public/AbilitySystem/RPGAttributeSetBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UI/RPGFloatingStatusBarWidget.h"

ARPGEnemy::ARPGEnemy(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{


    GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
    UIFloatingStatusBarComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("UIFloatingStatusBar"));
    UIFloatingStatusBarComponent->SetupAttachment(RootComponent);
    UIFloatingStatusBarComponent->SetRelativeLocation(FVector(0,0,120));
    UIFloatingStatusBarComponent->SetWidgetSpace(EWidgetSpace::Screen);
    UIFloatingStatusBarComponent->SetDrawSize(FVector2D(500,500));

    UIFloatingStatusBarClass = StaticLoadClass(UObject::StaticClass(),nullptr,TEXT("/Game/RPG/UI/WB_FloatingStatusBarHero.WB_FloatingStatusBarHero_C"));

    if (UIFloatingStatusBarClass)
    {
        //We print the function that this class belongs to and then we print the message to alert the uset that the class has not been uptaded from the folder.
        UE_LOG(LogTemp, Error,TEXT(" %s() , The UIFloatingBar Class have not been found. "),*FString(__FUNCTION__));
    }
    
}

URPGFloatingStatusBarWidget* ARPGEnemy::GetFlopFloatingStatusBarWidget()
{
    return UIFloatingStatusBar;
}


void ARPGEnemy::InitializeFloatingBar()
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(),0);
    if (PC && PC->IsLocalController())
    {
        if (UIFloatingStatusBarClass)
        {
            UIFloatingStatusBar = CreateWidget<URPGFloatingStatusBarWidget>(PC , UIFloatingStatusBarClass);

            if (UIFloatingStatusBar && UIFloatingStatusBarComponent)
            {
                UIFloatingStatusBarComponent->SetWidget(UIFloatingStatusBar);


                // We Assign the Healt to the Max Health here.
                    

                UIFloatingStatusBar->SetHealthPercentage(GetHealth() /GetMaxHealth());

                   
            }
        }
    }
}

void ARPGEnemy::BeginPlay()
{
    Super::BeginPlay();


    ARPGPlayerState* PS = Cast<ARPGPlayerState>(GetPlayerState());

    if (PS)
    {
        // We set the Ability System Component and the Attribute Set Base from the Player State.
        AttributeSet = PS->GetAttributeSetBase();

        AbilitySystemComponent = Cast<URPGAbilitySystemComponent>(PS->GetAbilitySystemComponent());

        PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS,this);
    }

    if (AbilitySystemComponent.IsValid())
    {
        AbilitySystemComponent->InitAbilityActorInfo(this,this);
        InitializeAbilities();
        InitializeAttributes();

        InitializeFloatingBar();
    }

    HealthChangedDelegate = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this,&ARPGEnemy::Healthchanged);

    // Team Setting


    if (PS)
    {
        PS->SetTeam(ERPGTeam::Enemy);
    }
    
}



void ARPGEnemy::Healthchanged(const FOnAttributeChangeData& Data)
{

    float NewValue = 0.0f;

    NewValue = Data.NewValue;

    if (UIFloatingStatusBar)
    {
        UIFloatingStatusBar->SetHealthPercentage(NewValue / GetMaxHealth());
    }
}
