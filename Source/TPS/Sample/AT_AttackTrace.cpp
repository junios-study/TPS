// Fill out your copyright notice in the Description page of Project Settings.


#include "Sample/AT_AttackTrace.h"
#include "GATA_AttackTrace.h"
#include "AbilitySystemComponent.h"

UAT_AttackTrace::UAT_AttackTrace()
{
}

void UAT_AttackTrace::Activate()
{
    Super::Activate();

    UE_LOG(LogTemp, Display, TEXT("UAT_AttackTrace::Activate"));

    AGATA_AttackTrace* SpawnedActor = Ability->GetWorld()->SpawnActorDeferred<AGATA_AttackTrace>(TargetActorClass,
        FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
    if (SpawnedActor)
    {
        SpawnedActor->TargetDataReadyDelegate.AddUObject(this, &UAT_AttackTrace::CompleteTargetDataReady);
    }

    UAbilitySystemComponent* ASC = AbilitySystemComponent.Get();
    if (ASC)
    {
        const FTransform SpawnTransform = ASC->GetAvatarActor()->GetTransform();

        SpawnedActor->FinishSpawning(SpawnTransform);
        ASC->SpawnedTargetActors.Push(SpawnedActor);
        SpawnedActor->StartTargeting(Ability);
        SpawnedActor->ConfirmTargeting();
    }
}

void UAT_AttackTrace::OnDestroy(bool bInOwnerFinished)
{
}

UAT_AttackTrace* UAT_AttackTrace::CreateTask(UGameplayAbility* OwingAbility, TSubclassOf<class AGATA_AttackTrace> TargetActorClass)
{
    UAT_AttackTrace* NewTask = NewAbilityTask<UAT_AttackTrace>(OwingAbility);
    NewTask->TargetActorClass = TargetActorClass;

    return NewTask;
}

void UAT_AttackTrace::CompleteTargetDataReady(const FGameplayAbilityTargetDataHandle& Handle)
{
    //Send Ability
    if (ShouldBroadcastAbilityTaskDelegates())
    {
        OnCompleted.Broadcast(Handle);
    }
    
    EndTask();
}
