// Fill out your copyright notice in the Description page of Project Settings.


#include "Sample/GameplayAbility_CharacterJump2.h"
#include "AbilityTask_WaitLanded.h"

void UGameplayAbility_CharacterJump2::OnLanded(const FHitResult& HitResult)
{
    // ĳ���Ͱ� �����ϸ� �� �Լ��� ȣ��˴ϴ�.
    // ���⼭ ���� �����Ƽ�� ����մϴ�.
    //CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
    EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

bool UGameplayAbility_CharacterJump2::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	return Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
}

void UGameplayAbility_CharacterJump2::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* OwnerInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, OwnerInfo, ActivationInfo, TriggerEventData);

    // �����Ƽ�� Ȱ��ȭ�Ǹ� WaitLanded �½�ũ�� �����ϰ� �����մϴ�.
    LandedTask = UAbilityTask_WaitLanded::WaitLanded(this);
    // �½�ũ�� OnLanded ��������Ʈ�� �� �����Ƽ�� �Լ��� ���ε��մϴ�.
    LandedTask->OnLanded.AddDynamic(this, &UGameplayAbility_CharacterJump2::OnLanded);
    // �½�ũ�� ���� �غ� ���·� ����ϴ�.
    LandedTask->ReadyForActivation();
}

void UGameplayAbility_CharacterJump2::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{

}

void UGameplayAbility_CharacterJump2::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
    Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}
