// Fill out your copyright notice in the Description page of Project Settings.


#include "Sample/AbilityTask_WaitLanded.h"

#include "GameFramework/Character.h"

UAbilityTask_WaitLanded* UAbilityTask_WaitLanded::WaitLanded(UGameplayAbility* OwningAbility)
{
    // ���ο� �½�ũ �ν��Ͻ��� �����մϴ�.
    UAbilityTask_WaitLanded* MyTask = NewAbilityTask<UAbilityTask_WaitLanded>(OwningAbility);
    return MyTask;
}

ACharacter* UAbilityTask_WaitLanded::GetTargetCharacter() const
{
    // �����Ƽ�� �ƹ�Ÿ ����(ĳ����)�� �����ɴϴ�.
    return Cast<ACharacter>(Ability->GetAvatarActorFromActorInfo());
}

void UAbilityTask_WaitLanded::Activate()
{
    Super::Activate();

    ACharacter* Character = Cast<ACharacter>(Ability->GetAvatarActorFromActorInfo());
    if (Character)
    {
        // ĳ������ OnLanded ��������Ʈ�� �ݹ� �Լ��� ���ε��մϴ�.
        Character->LandedDelegate.AddDynamic(this, &UAbilityTask_WaitLanded::OnLandedCallback);
    }
    else
    {
        // ĳ���͸� ã�� �� ������ �½�ũ�� �����մϴ�.
        EndTask();
    }
}

void UAbilityTask_WaitLanded::OnDestroy(bool AbilityEnded)
{
    ACharacter* Character = GetTargetCharacter();
    if (Character)
    {
        // ��������Ʈ ���ε��� �����ϰ� �����Ͽ� �޸� ������ �����մϴ�.
        Character->LandedDelegate.RemoveDynamic(this, &UAbilityTask_WaitLanded::OnLandedCallback);
    }

    Super::OnDestroy(AbilityEnded);
}

void UAbilityTask_WaitLanded::OnLandedCallback(const FHitResult& HitResult)
{
    // OnLanded ��������Ʈ�� ȣ��Ǿ����Ƿ�, �� �½�ũ�� ��� ��������Ʈ�� OnLanded�� ��ε�ĳ��Ʈ�մϴ�.
    if (ShouldBroadcastAbilityTaskDelegates())
    {
        OnLanded.Broadcast(HitResult);
    }
    // �̺�Ʈ�� ���������Ƿ� �½�ũ�� ������ ���߽��ϴ�. �½�ũ�� �����մϴ�.
    EndTask();
}
