// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG/Public/AbilitySystem/ExecutionCalculations/RPGWarriorExecutionCalculation.h"
#include "RPG/Public/AbilitySystem/RPGAbilitySystemComponent.h"
#include "AbilitySystem/RPGAttributeSetBase.h"


//First we declare the attributes that we want to capture witha  struct.

struct RPGDamageStatics
{
    DECLARE_ATTRIBUTE_CAPTUREDEF(Armor)

    // In this case we are not capturing the Damage exactly because we are getting the damage amount via Set by caller with the Effect Spec, but it is neccessary to just declare in order to modify it.
    DECLARE_ATTRIBUTE_CAPTUREDEF(Damage)

    DECLARE_ATTRIBUTE_CAPTUREDEF(LifeSteal)


    RPGDamageStatics()
    {

        // Capture the Target's Armor. Don't snapshot (the false).
        DEFINE_ATTRIBUTE_CAPTUREDEF(URPGAttributeSetBase,Armor,Target,false);


        // The Target's received Damage. This is the value of health that will be subtracted on the Target. We're not capturing this.
        DEFINE_ATTRIBUTE_CAPTUREDEF(URPGAttributeSetBase,Damage,Target,false);

        DEFINE_ATTRIBUTE_CAPTUREDEF(URPGAttributeSetBase,LifeSteal,Source,true);
    }
};

static const RPGDamageStatics& DamageStatics()
{
    static RPGDamageStatics DamageStatics;
    return DamageStatics;
}


URPGWarriorExecutionCalculation::URPGWarriorExecutionCalculation()
{

    // We declare the attributes that are relevant to capture.In this case the Damage is not included because it is formed here on the ExecCalc.
    RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);

    RelevantAttributesToCapture.Add(DamageStatics().DamageDef);

    RelevantAttributesToCapture.Add(DamageStatics().LifeStealDef);
    
}

void URPGWarriorExecutionCalculation::Execute_Implementation(
    const FGameplayEffectCustomExecutionParameters& ExecutionParams,
    FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{

    UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
    UAbilitySystemComponent* TargetASC =ExecutionParams.GetTargetAbilitySystemComponent();

    const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

    //We get the  Tags from the Target and from the Source
    const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
    const FGameplayTagContainer* Targettags = Spec.CapturedTargetTags.GetAggregatedTags();


    // We relate the evaluated parameters with the Source and Target Tags
    FAggregatorEvaluateParameters EvaluateParameters;
    EvaluateParameters.SourceTags = SourceTags;
    EvaluateParameters.TargetTags = Targettags;

    // Now we get the Attributes Value

    float Armor = 0.0f;

    float Damage = 0.0f;
    
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef,EvaluateParameters,Armor);
    Armor = FMath::Max<float>(Armor,0.0f);

    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DamageDef,EvaluateParameters,Damage);

    //Set By Caller Damage. I opted to add a set by caller magnitude here because is a way cleaner for the designers and is more customizable form the BPs.

     float CallerDamage = FMath::Max<float>(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")),false,-1),0.0f);

    // If the Set By Caller Damage is greater than 0, then we use it, if not, we use the Damage on  the Execution Calculation caputured Modifiers.
    if (CallerDamage > 0)
    {
        Damage = CallerDamage;
    }

    float InitialDamage = Damage;

    //We calculate our final damage based on the Armor that the Target has.
    float FinalDamage = (InitialDamage) * (100/100 + Armor );

    if (FinalDamage > 0.f)
    {
        OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().DamageProperty,EGameplayModOp::Additive,FinalDamage));
    }

    //We Broadcast the Damage to the Target

    URPGAbilitySystemComponent* TargetHeroASC = Cast<URPGAbilitySystemComponent>(TargetASC);

    if (TargetHeroASC)
    {
        TargetHeroASC->ReceiveDamage(TargetHeroASC,InitialDamage,FinalDamage);
    }
    
}
