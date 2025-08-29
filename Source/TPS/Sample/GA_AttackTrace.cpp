// Fill out your copyright notice in the Description page of Project Settings.


#include "Sample/GA_AttackTrace.h"
#include "GATA_AttackTrace.h"
#include "AT_AttackTrace.h"
#include "GA_AttackTrace.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "CombatDamageable.h"

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
	NewTask->OnCompleted.AddDynamic(this, &UGA_AttackTrace::OnCompleted);
	NewTask->ReadyForActivation();
}

void UGA_AttackTrace::OnCompleted(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	UE_LOG(LogTemp, Display, TEXT("UGA_AttackTrace::OnCompleted"));

	if (UAbilitySystemBlueprintLibrary::TargetDataHasHitResult(TargetDataHandle, 0))
	{
		FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetDataHandle, 0);
		ICombatDamageable* DamagedActor = Cast<ICombatDamageable>(HitResult.GetActor());
		if (DamagedActor)
		{
			const FVector Impluse = (-HitResult.ImpactNormal * 300.0f) +
				(FVector::UpVector * 300.0f);
			DamagedActor->ApplyDamage(10, CurrentActorInfo->AvatarActor.Get(), 
				HitResult.ImpactPoint, Impluse);
		}

			
	}

	EndAbility(CurrentSpecHandle,
		CurrentActorInfo, 
		CurrentActivationInfo, true, false);
}
