// Fill out your copyright notice in the Description page of Project Settings.


#include "Sample/GameplayAbility_CharacterJump2.h"
#include "AbilityTask_WaitLanded.h"

void UGameplayAbility_CharacterJump2::OnLanded(const FHitResult& HitResult)
{
    // 캐릭터가 착지하면 이 함수가 호출됩니다.
    // 여기서 현재 어빌리티를 취소합니다.
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

    // 어빌리티가 활성화되면 WaitLanded 태스크를 생성하고 시작합니다.
    LandedTask = UAbilityTask_WaitLanded::WaitLanded(this);
    // 태스크의 OnLanded 델리게이트에 이 어빌리티의 함수를 바인딩합니다.
    LandedTask->OnLanded.AddDynamic(this, &UGameplayAbility_CharacterJump2::OnLanded);
    // 태스크를 실행 준비 상태로 만듭니다.
    LandedTask->ReadyForActivation();
}

void UGameplayAbility_CharacterJump2::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{

}

void UGameplayAbility_CharacterJump2::CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility)
{
    Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}
