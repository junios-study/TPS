// Fill out your copyright notice in the Description page of Project Settings.


#include "Sample/AbilityTask_WaitLanded.h"

#include "GameFramework/Character.h"

UAbilityTask_WaitLanded* UAbilityTask_WaitLanded::WaitLanded(UGameplayAbility* OwningAbility)
{
    // 새로운 태스크 인스턴스를 생성합니다.
    UAbilityTask_WaitLanded* MyTask = NewAbilityTask<UAbilityTask_WaitLanded>(OwningAbility);
    return MyTask;
}

ACharacter* UAbilityTask_WaitLanded::GetTargetCharacter() const
{
    // 어빌리티의 아바타 액터(캐릭터)를 가져옵니다.
    return Cast<ACharacter>(Ability->GetAvatarActorFromActorInfo());
}

void UAbilityTask_WaitLanded::Activate()
{
    Super::Activate();

    ACharacter* Character = Cast<ACharacter>(Ability->GetAvatarActorFromActorInfo());
    if (Character)
    {
        // 캐릭터의 OnLanded 델리게이트에 콜백 함수를 바인딩합니다.
        Character->LandedDelegate.AddDynamic(this, &UAbilityTask_WaitLanded::OnLandedCallback);
    }
    else
    {
        // 캐릭터를 찾을 수 없으면 태스크를 종료합니다.
        EndTask();
    }
}

void UAbilityTask_WaitLanded::OnDestroy(bool AbilityEnded)
{
    ACharacter* Character = GetTargetCharacter();
    if (Character)
    {
        // 델리게이트 바인딩을 안전하게 해제하여 메모리 누수를 방지합니다.
        Character->LandedDelegate.RemoveDynamic(this, &UAbilityTask_WaitLanded::OnLandedCallback);
    }

    Super::OnDestroy(AbilityEnded);
}

void UAbilityTask_WaitLanded::OnLandedCallback(const FHitResult& HitResult)
{
    // OnLanded 델리게이트가 호출되었으므로, 이 태스크의 출력 델리게이트인 OnLanded를 브로드캐스트합니다.
    if (ShouldBroadcastAbilityTaskDelegates())
    {
        OnLanded.Broadcast(HitResult);
    }
    // 이벤트를 감지했으므로 태스크의 역할을 다했습니다. 태스크를 종료합니다.
    EndTask();
}
