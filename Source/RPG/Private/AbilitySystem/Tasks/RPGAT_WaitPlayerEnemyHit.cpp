// Ishak Hero.All Rights Reserved.


#include "RPG/Public/AbilitySystem/Tasks/RPGAT_WaitPlayerEnemyHit.h"

#include "RPGEnemy.h"
#include "Character/RPGPlayerState.h"

URPGAT_WaitPlayerEnemyHit* URPGAT_WaitPlayerEnemyHit::WaitPlayerEnemyHit(UAbilitySystemComponent* PlayerASC)
{

    // We create the Tak
    URPGAT_WaitPlayerEnemyHit* WaitPlayerEnemyHit = NewObject<URPGAT_WaitPlayerEnemyHit>();

    WaitPlayerEnemyHit->ASC = PlayerASC;

    if (!IsValid(PlayerASC))
    {
        WaitPlayerEnemyHit->RemoveFromRoot();

        return nullptr;
    }


   PlayerASC->OnGameplayEffectAppliedDelegateToTarget.AddUObject(WaitPlayerEnemyHit,&URPGAT_WaitPlayerEnemyHit::OnEnemyHitCallBack);
   PlayerASC->OnGameplayEffectAppliedDelegateToSelf.AddUObject(WaitPlayerEnemyHit,&URPGAT_WaitPlayerEnemyHit::OnPlayerHitCallBack);
    
    return WaitPlayerEnemyHit;
    
}

void URPGAT_WaitPlayerEnemyHit::EndTask()
{

    // Removals when ending then Ability Task. You may use this when you want to end the listening delegates for of the task(Attribute listener.)
}

void URPGAT_WaitPlayerEnemyHit::OnPlayerHitCallBack(UAbilitySystemComponent* AbilitySystemComponent,
    const FGameplayEffectSpec& Spec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
{

    ARPGCharacterBase* InstigatorCharacter = nullptr;
    ARPGCharacterBase* HitCharacter = nullptr;
    
    if (Spec.GetContext().GetHitResult())
    {
        if (Spec.GetContext().GetHitResult()->GetActor())
        {
            
            InstigatorCharacter = Cast<ARPGCharacterBase>(Spec.GetContext().GetInstigator());
            
            if (Spec.GetContext().GetAbility()->AbilityTags.HasAny(FGameplayTag::RequestGameplayTag(FName("Ability.CanInitBattle")).GetSingleTagContainer()))
            {
        
                    HitCharacter = Cast<ARPGCharacterBase>(Spec.GetContext().GetHitResult()->GetActor());
                    OnBattleHitEvent.Broadcast(InstigatorCharacter,HitCharacter,ERPGHitType::PlayerWasHit);
                
            }
        }
    }
    
}

void URPGAT_WaitPlayerEnemyHit::OnEnemyHitCallBack(UAbilitySystemComponent* AbilitySystemComponent,
    const FGameplayEffectSpec& Spec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
{

    FGameplayEffectContextHandle ContextHandle;
    FHitResult ContexthandleHitResult;
    const UGameplayAbility* ContextHandleAbility;

    ARPGCharacterBase* HitCharacter = nullptr;
    ARPGPlayerState* ASCOwner = nullptr;
    ARPGCharacterBase* InstigatorCharacter = nullptr;
    
  if (Spec.GetContext().Get())
  {
    ContextHandle = Spec.GetContext();
      
    if (ContextHandle.IsValid())
    {
        if (ContextHandle.GetHitResult())
        {
            ContexthandleHitResult = *ContextHandle.GetHitResult();
          
            if ( ( ContexthandleHitResult.GetActor() ) && ( ContextHandle.GetAbility() ) )
            {
                if (ContextHandle.GetInstigator())
                {
                    UE_LOG(LogTemp , Error , TEXT(" LOLLL %s"),*ContextHandle.GetInstigator()->GetName());
                    ASCOwner =Cast<ARPGPlayerState>( ContextHandle.GetInstigator() );
                    
                    if (ASCOwner)
                    {
                        InstigatorCharacter = Cast<ARPGCharacterBase>(ASCOwner->GetPawn());
                    }
                    
                    HitCharacter = Cast<ARPGCharacterBase>(ContexthandleHitResult.GetActor());
                }
           
                ContextHandleAbility = ContextHandle.GetAbility();
 
                if (ContextHandleAbility->AbilityTags.HasAny(FGameplayTag::RequestGameplayTag(FName("Ability.CanInitBattle")).GetSingleTagContainer()))
                {
                    if (InstigatorCharacter && HitCharacter)
                    {
                        OnBattleHitEvent.Broadcast( InstigatorCharacter , HitCharacter,ERPGHitType::EnemyWasHit);
                    }
                    
                }
            }
        }
    }

  }

}

