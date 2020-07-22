// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG/Public/AbilitySystem/Tasks/RPGAT_ListenForAttributeChange.h"

URPGAT_ListenForAttributeChange* URPGAT_ListenForAttributeChange::ListenForAttributeChange(FGameplayAttribute Attribute,UAbilitySystemComponent* AbilitySystemComponent)
{

    // We create an empty Task First
    URPGAT_ListenForAttributeChange* ListenForAttributeChangeTask = NewObject<URPGAT_ListenForAttributeChange>();
    ListenForAttributeChangeTask->ASC = AbilitySystemComponent;
    ListenForAttributeChangeTask->AttributeToListenFor = Attribute;
    

    if (!IsValid(AbilitySystemComponent) || !Attribute.IsValid())
    {
        ListenForAttributeChangeTask->RemoveFromRoot();
        return  nullptr;
    }

    // We get the Attribute Change Delegate for later usage
    FOnGameplayAttributeValueChange ASCOnAttributeChange = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute);

    // We assign the Delegate to our function
    ASCOnAttributeChange.AddUObject(ListenForAttributeChangeTask,&URPGAT_ListenForAttributeChange::OnAttributeChanged);

    return ListenForAttributeChangeTask;
}

URPGAT_ListenForAttributeChange* URPGAT_ListenForAttributeChange::ListenForAttributesChange(
    TArray<FGameplayAttribute> Attributes, UAbilitySystemComponent* AbilitySystemComponent)
{
    URPGAT_ListenForAttributeChange* ListenForAttributeChangeTask = NewObject<URPGAT_ListenForAttributeChange>();
    ListenForAttributeChangeTask->ASC = AbilitySystemComponent;
    ListenForAttributeChangeTask->AttributesToListenFor = Attributes;

    if (!IsValid(AbilitySystemComponent) || Attributes.Num()< 1)
    {
        ListenForAttributeChangeTask->RemoveFromRoot();
        return  nullptr;
    }


    for (FGameplayAttribute Attribute : ListenForAttributeChangeTask->AttributesToListenFor)
    {
        if (Attribute.IsValid())
        {
           FOnGameplayAttributeValueChange OnGameplayAttributeValueChange =  AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute);

            OnGameplayAttributeValueChange.AddUObject(ListenForAttributeChangeTask,&URPGAT_ListenForAttributeChange::OnAttributeChanged);
        }
    }

    return ListenForAttributeChangeTask;
}


void URPGAT_ListenForAttributeChange::OnAttributeChanged(const FOnAttributeChangeData& Data)
{
    // We Broadcast the Delegate
    OnAttributeChangedDelegate.Broadcast(Data.Attribute,Data.OldValue,Data.NewValue);
}

void URPGAT_ListenForAttributeChange::EndTask()
{
    if (IsValid(ASC))
    {
        ASC->GetGameplayAttributeValueChangeDelegate(AttributeToListenFor).RemoveAll(this);

        for (FGameplayAttribute Attribute : AttributesToListenFor)
        {
            ASC->GetGameplayAttributeValueChangeDelegate(Attribute).RemoveAll(this);
        }
    }
}

