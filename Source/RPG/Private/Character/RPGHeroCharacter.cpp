// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG/Public/Character/RPGHeroCharacter.h"


#include "Components/CapsuleComponent.h"
#include "RPG/Public/Character/RPGPlayerState.h"
#include "RPG/Public/Character/RPGPlayerController.h"
#include "RPG/Public/AbilitySystem/RPGAttributeSetBase.h"
#include "RPG/Public/AbilitySystem/RPGAbilitySystemComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/RPGAbilitySystemComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "RPG/RPGGameMode.h"
#include "Kismet/KismetMathLibrary.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/DecalComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "UObject/ConstructorHelpers.h"




ARPGHeroCharacter::ARPGHeroCharacter(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(FName("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->SetRelativeLocation(FVector(0, 0, 68.492264));

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(FName("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom);
	FollowCamera->FieldOfView = 80.0f;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	// Makes sure that the animations play on the Server so that we can use bone and socket transforms
	// to do things like spawning projectiles and other FX.
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionProfileName(FName("NoCollision"));


	UIFloatingStatusBarComponent = CreateDefaultSubobject<UWidgetComponent>(FName("FloatingBarUI"));

	UIFloatingStatusBarComponent->SetupAttachment(RootComponent);
	UIFloatingStatusBarComponent->SetRelativeLocation(FVector(0,0,120));
	UIFloatingStatusBarComponent->SetWidgetSpace(EWidgetSpace::Screen);
	UIFloatingStatusBarComponent->SetDrawSize(FVector2D(500,500));

	UIFloatingStatusBarClass = StaticLoadClass(UObject::StaticClass(),nullptr,TEXT("/Game/RPG/UI/WB_FloatingStatusBarHero.WB_FloatingStatusBarHero_C"));

	if (!UIFloatingStatusBarClass)
	{
		UE_LOG(LogTemp,Error,TEXT(" %s() The UI Floating bar has not been loaded please update the reference location of the file on the c++ RPGHeroCharacter"),*FString(__FUNCTION__));
	}
	

}

void ARPGHeroCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(ARPGHeroCharacter, CharacterAnimationMode, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ARPGHeroCharacter, CharacterHeroType, COND_None, REPNOTIFY_Always);
}

// Called to bind functionality to input
void ARPGHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	

	PlayerInputComponent->BindAxis("MoveForward", this, &ARPGHeroCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARPGHeroCharacter::MoveRight);

	PlayerInputComponent->BindAxis("LookUp", this, &ARPGHeroCharacter::LookUp);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ARPGHeroCharacter::LookUpRate);
	PlayerInputComponent->BindAxis("Turn", this, &ARPGHeroCharacter::Turn);
	PlayerInputComponent->BindAxis("TurnRate", this, &ARPGHeroCharacter::TurnRate);

	// Bind player input to the AbilitySystemComponent. Also called in OnRep_PlayerState because of a potential race condition.
	BindASCInput();
}
/**
* On the Server, Possession happens before BeginPlay.
* On the Client, BeginPlay happens before Possession.
* So we can't use BeginPlay to do anything with the AbilitySystemComponent because we don't have it until the PlayerState replicates from possession.
*/
void ARPGHeroCharacter::BeginPlay()
{
	Super::BeginPlay();


	//We will initiate the Floating Status Bar here just for Player that are on the Server, the Players that are clients Initialize their Status Bar on the OnRep_PlayerState;

	InitializeFloatingStatusBar();

	StartingCameraBoomArmLenght = CameraBoom->TargetArmLength;
	StartingCameraBoomLocation = CameraBoom->GetRelativeLocation();


}

// Server only
void ARPGHeroCharacter::PossessedBy(AController * NewController)
{
	Super::PossessedBy(NewController);

	ARPGPlayerState* PS = GetPlayerState<ARPGPlayerState>();
	if (PS)
	{
		// Set the ASC on the Server. Clients do this in OnRep_PlayerState()
		AbilitySystemComponent = Cast<URPGAbilitySystemComponent>(PS->GetAbilitySystemComponent());

		// AI won't have PlayerControllers so we can init again here just to be sure. No harm in initing twice for heroes that have PlayerControllers.
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);

		// Set the AttributeSetBase for convenience attribute functions
		AttributeSet = PS->GetAttributeSetBase();

		// If we handle players disconnecting and rejoining in the future, we'll have to change this so that possession from rejoining doesn't reset attributes.
		// For now assume possession = spawn/respawn.
		InitializeAttributes();

		InitializeAbilities();

		ARPGPlayerController* PC = Cast<ARPGPlayerController>(GetController());
		if (PC)
		{
			PC->CreateHUD();
		}


		
		// Respawn specific things that won't affect first possession.
		// Set Health/Mana/Stamina to their max. This is only necessary for *Respawn*.
		SetHealth(GetMaxHealth());

	
		PS->SetTeam(PlayerTeam);

		InitializeFloatingStatusBar();
	}
}

// Client only
void ARPGHeroCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	ARPGPlayerState* PS = GetPlayerState<ARPGPlayerState>();
	if (PS)
	{
		// Set the ASC for clients. Server does this in PossessedBy.
		AbilitySystemComponent = Cast<URPGAbilitySystemComponent>(PS->GetAbilitySystemComponent());

		// Init ASC Actor Info for clients. Server will init its ASC when it possesses a new Actor.
		AbilitySystemComponent->InitAbilityActorInfo(PS, this);

		// Bind player input to the AbilitySystemComponent. Also called in SetupPlayerInputComponent because of a potential race condition.
		BindASCInput();

		// Set the AttributeSetBase for convenience attribute functions
		AttributeSet = PS->GetAttributeSetBase();

		// If we handle players disconnecting and rejoining in the future, we'll have to change this so that posession from rejoining doesn't reset attributes.
		// For now assume possession = spawn/respawn.
		InitializeAttributes();

		ARPGPlayerController* PC = Cast<ARPGPlayerController>(GetController());

		if (PC)
		{
			PC->CreateHUD();
		}
	
		// Respawn specific things that won't affect first possession.

		

		// Set Health/Mana/Stamina to their max. This is only necessary for *Respawn*.
		SetHealth(GetMaxHealth());

	
		PS->SetTeam(PlayerTeam);

		InitializeFloatingStatusBar();

		

	}
}

void ARPGHeroCharacter::SetCharacterHeroType( ERPGCharacterHeroType NewCharacterHeroType)
{
	this->CharacterHeroType = NewCharacterHeroType;
}

void ARPGHeroCharacter::SetCharacterAnimationMode(  ERPGAnimationMode NewCharacterAnimationMode)
{
	this->CharacterAnimationMode = NewCharacterAnimationMode;
}

ERPGCharacterHeroType ARPGHeroCharacter::GetCharacterHeroType() const
{
	return CharacterHeroType;
}

ERPGAnimationMode ARPGHeroCharacter::GetCharacterAnimationMode() const
{
	return CharacterAnimationMode;
}

USpringArmComponent * ARPGHeroCharacter::GetCameraBoom()
{
	return CameraBoom;
}

UCameraComponent * ARPGHeroCharacter::GetFollowCamera()
{
	return FollowCamera;
}

float ARPGHeroCharacter::GetStartingSpringCameraBoomArmLenght()
{
	return StartingCameraBoomArmLenght;
}

FVector ARPGHeroCharacter::GetStartingCameraBoomLocation()
{
	return StartingCameraBoomLocation;
}

ERPGTeam ARPGHeroCharacter::GetTeam() const
{
	return PlayerTeam;
}

URPGFloatingStatusBarWidget* ARPGHeroCharacter::GetFloatingStatusBar()
{
	return IsValid(UIFloatingStatusBar) ? UIFloatingStatusBar : nullptr;
}

void ARPGHeroCharacter::InitializeFloatingStatusBar()
{
	// We will create the widget just one time
	if (UIFloatingStatusBar || !GetAbilitySystemComponent())
	{
		return;
	}

	//We just set up the UI for the Players that are local ones, not for any copy on the server or other things. The function that we use here is the same that we use on the Blueprints.
	ARPGPlayerController* PC = Cast<ARPGPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(),0));

	if (UIFloatingStatusBarClass)
	{
		// We safely create the Widget on the Player Controller.
		UIFloatingStatusBar = CreateWidget<URPGFloatingStatusBarWidget>(PC,UIFloatingStatusBarClass);
		if (UIFloatingStatusBar && UIFloatingStatusBarComponent)
		{
			UIFloatingStatusBarComponent->SetWidget(UIFloatingStatusBar);

			UIFloatingStatusBar->SetHealthPercentage(GetHealth() / GetMaxHealth());
			//TODO add the mana Percentage

			
		}
	}
	

	
	
}

void ARPGHeroCharacter::LookUp(float Value)
{
	if (IsAlive())
	{
		AddControllerPitchInput(Value);

		
	}
}

void ARPGHeroCharacter::LookUpRate(float Value)
{
	if (IsAlive())
	{
		//We calculate the cm/frame
		float fLookUpRatePerFrame = GetWorld()->DeltaTimeSeconds * BaseTurnRate;

		// We multyply by the sensitivity
		float fFinalLookUpRate = fLookUpRatePerFrame * Value;

		AddControllerPitchInput(fFinalLookUpRate);
	}
}

void ARPGHeroCharacter::Turn(float Value)
{
	if (IsAlive())
	{
		AddControllerYawInput(Value);
	}
}

void ARPGHeroCharacter::TurnRate(float Value)
{
	if (IsAlive())
	{
		// We get the cm/frame
		float fTurnRatePerFrame = GetWorld()->DeltaTimeSeconds * BaseTurnRate;

		// And we multiply by the sensitivity
		float fFinalTurnRate = fTurnRatePerFrame * Value;

		AddControllerYawInput(fFinalTurnRate);

	}
}

void ARPGHeroCharacter::MoveForward(float Value)
{
	AddMovementInput(UKismetMathLibrary::GetForwardVector(FRotator(0, GetControlRotation().Yaw, 0)), Value);
}

void ARPGHeroCharacter::MoveRight(float Value)
{
	AddMovementInput(UKismetMathLibrary::GetRightVector(FRotator(0, GetControlRotation().Yaw, 0)), Value);
}





void ARPGHeroCharacter::BindASCInput()
{
	if (!ASCInputBound && AbilitySystemComponent.IsValid() && IsValid(InputComponent))
	{
		AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, FGameplayAbilityInputBinds(FString("ConfirmTarget"),
			FString("CancelTarget"), FString("ERPGbilityInputID"), static_cast<int32>(ERPGbilityInputID::Confirm), static_cast<int32>(ERPGbilityInputID::Cancel)));

		ASCInputBound = true;
	}
}

