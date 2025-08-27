// Fill out your copyright notice in the Description page of Project Settings.


#include "Sample/GA_AttackTrace.h"
#include "GATA_AttackTrace.h"
#include "AT_AttackTrace.h"
#include "GA_AttackTrace.h"

UGA_AttackTrace::UGA_AttackTrace()
{
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateYes;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_AttackTrace::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//Create Task
	UAT_AttackTrace* NewTask = UAT_AttackTrace::CreateTask(this, AGATA_AttackTrace::StaticClass());
	//NewTask->타격 판정 완료가 되면 알려주는 콜백
	NewTask->ReadyForActivation();
}
