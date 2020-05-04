// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGCharacterMovementComponent.h"

#include "RPGCharacterBase.h"

URPGCharacterMovementComponent::URPGCharacterMovementComponent()
{
    SprintMultiplier = 1.4f;   
}

float URPGCharacterMovementComponent::GetMaxSpeed() const
{
    ARPGCharacterBase* Player = Cast<ARPGCharacterBase>(GetOwner());

    if(!Player)
    {
        UE_LOG(LogTemp, Error, TEXT("%s() No Owner"), TEXT(__FUNCTION__));
        return Super::GetMaxSpeed();
    }

    if(!Player->IsAlive())
    {
        return  0.0f;
    }

    //TODO add the stun state for the player.

    if(RequestToStartSprinting)
    {
        return  Player->GetMoveSpeed() * SprintMultiplier;
    }

    return Player->GetMoveSpeed();
}

void URPGCharacterMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
    //The Flags parameter contains the compressed input flags that are stored in the saved move.
    //UpdateFromCompressed flags simply copies the flags from the saved move into the movement component.
    //It basically just resets the movement component to the state when the move was made so it can simulate from there.
    Super::UpdateFromCompressedFlags(Flags);

    RequestToStartSprinting = (Flags & FSavedMove_Character::FLAG_Custom_0) != 0;
    
}

FNetworkPredictionData_Client* URPGCharacterMovementComponent::GetPredictionData_Client() const
{
    check(PawnOwner != NULL);

    if(!ClientPredictionData)
    {
        URPGCharacterMovementComponent* MutableThis = const_cast<URPGCharacterMovementComponent*>(this);

        MutableThis->ClientPredictionData = new FRPGNetworkPredictionData_Client(*this);
        MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.f;
        MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.f;
    }

    return ClientPredictionData;
}

void URPGCharacterMovementComponent::StartSprinting()
{
    RequestToStartSprinting = true;
}

void URPGCharacterMovementComponent::StopSprinting()
{
     RequestToStartSprinting = false;
}

void URPGCharacterMovementComponent::FRPGSavedMode::Clear()
{
    Super::Clear();

    SavedRequestToStartSprinting = false;
}

uint8 URPGCharacterMovementComponent::FRPGSavedMode::GetCompressedFlags() const
{
    uint8 Result = Super::GetCompressedFlags();

    if(SavedRequestToStartSprinting)
    {
        Result |= FLAG_Custom_0;
    }

   // We may add some other flags to the project.
    return Result;
}

bool URPGCharacterMovementComponent::FRPGSavedMode::CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* Character, float MaxDelta) const
{

    //Set which moves can be combined together. This will depend on the bit flags that are used.
    if(SavedRequestToStartSprinting != ((FRPGSavedMode*)&NewMove)->SavedRequestToStartSprinting)
    {
        return false;
    }

    return Super::CanCombineWith(NewMove, Character, MaxDelta);
}

void URPGCharacterMovementComponent::FRPGSavedMode::SetMoveFor(ACharacter* Character, float InDeltaTime, FVector const& NewAccel, FNetworkPredictionData_Client_Character& ClientData)
{
    Super::SetMoveFor(Character, InDeltaTime, NewAccel, ClientData);

    URPGCharacterMovementComponent* CharacterMovement = Cast<URPGCharacterMovementComponent>(Character->GetCharacterMovement());

    if(CharacterMovement)
    {
        SavedRequestToStartSprinting = CharacterMovement->RequestToStartSprinting;
    }
}

void URPGCharacterMovementComponent::FRPGSavedMode::PrepMoveFor(ACharacter* Character)
{
    Super::PrepMoveFor(Character);

    URPGCharacterMovementComponent* CharacterMovementComponent = Cast<URPGCharacterMovementComponent>(Character->GetCharacterMovement());
    if(CharacterMovementComponent)
    {
        
    }
}

URPGCharacterMovementComponent::FRPGNetworkPredictionData_Client::FRPGNetworkPredictionData_Client(const UCharacterMovementComponent& ClientMovement)
    : Super(ClientMovement)
{
    
}

FSavedMovePtr URPGCharacterMovementComponent::FRPGNetworkPredictionData_Client::AllocateNewMove()
{
    return FSavedMovePtr(new FRPGSavedMode());
}














