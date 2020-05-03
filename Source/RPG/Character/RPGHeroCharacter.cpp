// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGHeroCharacter.h"
#include "Components/CapsuleComponent.h"
#include "RPGPlayerState.h"
#include "RPGPlayerController.h"
#include "RPG/AbilitySystem/RPGAttributeSetBase.h"
#include "RPG/RPGAbilitySystemComponent.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "RPG/RPGGameMode.h"
#include "Kismet/KismetMathLibrary.h"


ARPGHeroCharacter::ARPGHeroCharacter(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->SetRelativeLocation(FVector(0,0,CameraBoomDistanceToCharacter));

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom);
    FollowCamera->FieldOfView = 80.0f;

    GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

    // We make sure to play the animations on the server so we can manipulate the transforms of the bones and sockets and
    // cast FX and more things.
    GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    GetMesh()->SetCollisionProfileName(FName("NoCollision"));
    
}


void ARPGHeroCharacter::BeginPlay()
{
    StartingCameraBoomArmLenght = CameraBoom->TargetArmLength;
    StartingCameraBoomLocation = CameraBoom->GetRelativeLocation();
}

FVector ARPGHeroCharacter::GetStartingCameraBoomLocation()
{
    return StartingCameraBoomLocation;
}

float ARPGHeroCharacter::GetStartingSpringCameraBoomLenght()
{
    return StartingCameraBoomArmLenght;
}
void ARPGHeroCharacter::OnRep_PlayerState()
{
    // this is for the clients, not server.
    Super::OnRep_PlayerState();

    ARPGPlayerState* PS = Cast<ARPGPlayerState>(GetPlayerState());
    if(PS)
    {
     AbilitySystemComponent = Cast<URPGAbilitySystemComponent>(PS->GetAbilitySystemComponent());

        // We initialize the ability Actor Info once the  Ability System has been related.
        PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS,this);

     AttributeSet = PS->GetAttributeSetBase();

        InitializeAttributes();
        // Use when the Player Respawns or he joins the game.
        SetHealth(GetMaxHealth());

        //TODO Set the Map Tag Count
    }
}

void ARPGHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward",this,&ARPGHeroCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MOveRight",this,&ARPGHeroCharacter::MoveForward);

    PlayerInputComponent->BindAxis("LookUp",this,&ARPGHeroCharacter::LookUp);
    PlayerInputComponent->BindAxis("LookUpRate",this,&ARPGHeroCharacter::LookUpRate);
    PlayerInputComponent->BindAxis("Turn",this,&ARPGHeroCharacter::Turn);
    PlayerInputComponent->BindAxis("TurnRate",this,&ARPGHeroCharacter::TurnRate);
}


void ARPGHeroCharacter::PossessedBy(AController* NewController)
{
    // Server Only
    
    Super::PossessedBy(NewController);

    ARPGPlayerState* PS = Cast<ARPGPlayerState>(GetPlayerState());
    
    if(PS)
    {
        AbilitySystemComponent = Cast<URPGAbilitySystemComponent>(PS->GetAbilitySystemComponent());
        PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS,this);

        AttributeSet = PS->GetAttributeSetBase();

        InitializeAttributes();

        //TODO Add the character abilities only on the server;
        //TODO add the Dead tag Map Count to 0;

        
        SetHealth(GetMaxHealth());

        
    }
    
}

void ARPGHeroCharacter::PostInitializeComponents()
{
    Super::PostInitializeComponents();
}

void ARPGHeroCharacter::LookUp(float fValue)
{
    if(IsAlive())
    {
       AddControllerPitchInput(fValue); 
    }
}

void ARPGHeroCharacter::LookUpRate(float fValue)
{
    if(IsAlive())
    {
        // Siempre que se quiera comparar y llegar a medir una unidad en /frame hay que multiplicarla por deltatime seconds
        // que son los frames / s .
     float fLookUpRatePerFrame = GetWorld()->DeltaTimeSeconds * BaseLookUpRate;

     float fFinalLookUpRate = fLookUpRatePerFrame * fValue;

       AddControllerPitchInput(fFinalLookUpRate);
        
    }
}

void ARPGHeroCharacter::Turn(float fValue)
{
    if(IsAlive())
    {
        AddControllerYawInput(fValue);
    }
}

void ARPGHeroCharacter::TurnRate(float fValue)
{
    if (IsAlive())
    {
        float fTurnAmountperFrame = GetWorld()->DeltaTimeSeconds * BaseTurnRate;

        float fTurnAmount = fTurnAmountperFrame * fValue;
        
        AddControllerYawInput(fTurnAmount);
    }
}

void ARPGHeroCharacter::MoveForward(float fValue)
{

   const FRotator ForwardRotator = FRotator(0,GetControlRotation().Yaw,0);

    AddMovementInput(UKismetMathLibrary::GetForwardVector(ForwardRotator),fValue);

    
}

void ARPGHeroCharacter::MoveRight(float fValue)
{
    const FRotator RightRotator = FRotator(0,GetControlRotation().Yaw,0);

    AddMovementInput(UKismetMathLibrary::GetRightVector(RightRotator),fValue);
}
