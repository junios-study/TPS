// Fill out your copyright notice in the Description page of Project Settings.


#include "Sample/AT_AttackTrace.h"

UAT_AttackTrace::UAT_AttackTrace()
{
}

void UAT_AttackTrace::Activate()
{
}

void UAT_AttackTrace::OnDestroy(bool bInOwnerFinished)
{
}

UAT_AttackTrace* UAT_AttackTrace::CreateTask(UGameplayAbility* OwingAbility, TSubclassOf<class UGATA_AttackTrace> TargetActorClass)
{
    UAT_AttackTrace* NewTask = NewAbilityTask<UAT_AttackTrace>(OwingAbility);
    NewTask->TargetActorClass = TargetActorClass;

    return NewTask;
}
