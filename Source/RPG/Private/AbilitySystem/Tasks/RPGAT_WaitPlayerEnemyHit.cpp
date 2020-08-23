// Ishak Hero.All Rights Reserved.


#include "RPG/Public/AbilitySystem/Tasks/RPGAT_WaitPlayerEnemyHit.h"

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
    if (Spec.GetContext().GetHitResult())
    {
        if (Spec.GetContext().GetHitResult()->GetActor())
        {
            if (Spec.GetContext().GetAbility()->AbilityTags.HasAny(FGameplayTag::RequestGameplayTag(FName("Ability.CanInitBattle")).GetSingleTagContainer()))
            {
                OnPlayerHit.Broadcast();
            }
        }
    }
    
}

void URPGAT_WaitPlayerEnemyHit::OnEnemyHitCallBack(UAbilitySystemComponent* AbilitySystemComponent,
    const FGameplayEffectSpec& Spec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
{
    if (Spec.GetContext().GetHitResult())
    {
        if (Spec.GetContext().GetHitResult()->GetActor())
        {
            if (Spec.GetContext().GetAbility()->AbilityTags.HasAny(FGameplayTag::RequestGameplayTag(FName("Ability.CanInitBattle")).GetSingleTagContainer()))
            {
                OnEnemyHit.Broadcast();
            }
        }
    }

    
}

