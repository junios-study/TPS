// Fill out your copyright notice in the Description page of Project Settings.


#include "Sample/GameplayAbility_Bigger.h"
#include "GameplayTagContainer.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "GameplayAbility_ComboAttack.h"


UGameplayAbility_Bigger::UGameplayAbility_Bigger()
{
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateYes;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	//현재 Ability Tag가 됨, 즉 이름
	FGameplayTagContainer MyTags(FGameplayTag::RequestGameplayTag(FName(TEXT("Actor.Action.Bigger"))));
	SetAssetTags(MyTags);

	//능력을 사용하는 액터의 상태
	ActivationOwnedTags.AddTag(FGameplayTag::RequestGameplayTag(FName(TEXT("Actor.State.Bigger"))));
}

void UGameplayAbility_Bigger::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (ActorInfo && ActorInfo->AvatarActor.IsValid())
	{
		AActor* AvartarActor = ActorInfo->AvatarActor.Get();
		UE_LOG(LogTemp, Warning, TEXT("ActivateAbility AvartarActor %s"), *AvartarActor->GetName());

		FVector NewScale = AvartarActor->GetActorScale3D() * 10.0f;
		AvartarActor->SetActorScale3D(NewScale);

		//Make Task, Spawn Task
		//UAbilityTask_WaitDelay* BiggerWaitDelay = UAbilityTask_WaitDelay::WaitDelay(this, 3.0f);
		//BiggerWaitDelay->OnFinish.AddDynamic(this, &UGameplayAbility_Bigger::OnCompleted);
		//BiggerWaitDelay->ReadyForActivation();
	}
}

void UGameplayAbility_Bigger::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);

	if (ActorInfo && ActorInfo->AvatarActor.IsValid())
	{
		AActor* AvartarActor = ActorInfo->AvatarActor.Get();
		UE_LOG(LogTemp, Warning, TEXT("CancelAbility AvartarActor %s"), *AvartarActor->GetName());
	}
}

void UGameplayAbility_Bigger::OnCompleted()
{
	UE_LOG(LogTemp, Warning, TEXT("ActivateAbility OnCompleted"));

	FVector NewScale = CurrentActorInfo->AvatarActor->GetActorScale3D() / 10.0f;
	CurrentActorInfo->AvatarActor->SetActorScale3D(NewScale);

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}