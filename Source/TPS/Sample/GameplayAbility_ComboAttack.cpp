// Fill out your copyright notice in the Description page of Project Settings.


#include "Sample/GameplayAbility_ComboAttack.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "TPSCharacter.h"

UGameplayAbility_ComboAttack::UGameplayAbility_ComboAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateYes;
}

void UGameplayAbility_ComboAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	UE_LOG(LogTemp, Warning, TEXT("UGameplayAbility_ComboAttack::ActivateAbility"));

}

void UGameplayAbility_ComboAttack::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	UE_LOG(LogTemp, Warning, TEXT("UGameplayAbility_ComboAttack::EndAbility"));

	ATPSCharacter* Character = Cast<ATPSCharacter>(CurrentActorInfo->AvatarActor.Get());

	if (IsValid(Character))
	{
		Character->bIsAttacking = false;
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGameplayAbility_ComboAttack::ComboAttack(ATPSCharacter* Character)
{
	//Ã¹ °ø°Ý
	if (IsValid(Character))
	{
		Character->bIsAttacking = true;
		Character->ComboCount = 0;

		UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
			this,
			TEXT("ComboAttack"),
			Character->ComboAttackMontage,
			1.0f,
			Character->ComboSectionNames[Character->ComboCount]
		);

		Task->OnCompleted.AddDynamic(this, &UGameplayAbility_ComboAttack::OnCompleted);
		Task->OnInterrupted.AddDynamic(this, &UGameplayAbility_ComboAttack::OnInterrupted);
		Task->ReadyForActivation();
	}
}

void UGameplayAbility_ComboAttack::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	ATPSCharacter* Character = Cast<ATPSCharacter>(ActorInfo->AvatarActor.Get());

	if (IsValid(Character))
	{
		if (Character->bIsAttacking)
		{
			Character->CachedAttackInputTime = GetWorld()->GetTimeSeconds();
		}
		else
		{
			ComboAttack(Character);
		}
	}
}

void UGameplayAbility_ComboAttack::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
}

void UGameplayAbility_ComboAttack::OnCompleted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGameplayAbility_ComboAttack::OnInterrupted()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGameplayAbility_ComboAttack::JumpToSection()
{
	if (CurrentActorInfo)
	{
		ATPSCharacter* Character = Cast<ATPSCharacter>(CurrentActorInfo->AvatarActor.Get());

		if (IsValid(Character))
		{
			UE_LOG(LogTemp, Warning, TEXT("MontageJumpToSection"));

			MontageJumpToSection(Character->ComboSectionNames[Character->ComboCount]);
		}
	}
}
