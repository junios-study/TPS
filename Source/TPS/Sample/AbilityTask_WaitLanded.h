// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_WaitLanded.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitLandedDelegate, const FHitResult&, HitResult);

/**
 * 
 */
UCLASS()
class TPS_API UAbilityTask_WaitLanded : public UAbilityTask
{
	GENERATED_BODY()
public:
    // �������Ʈ�� C++���� ȣ���� �� �ִ� �½�ũ�� ��� ���Դϴ�.
    UPROPERTY(BlueprintAssignable)
    FWaitLandedDelegate OnLanded;

    // �� �½�ũ�� �����ϰ� Ȱ��ȭ�ϴ� ���� �Լ��Դϴ�. �������Ʈ ����� ���� �� ������ �մϴ�.
    UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "Wait For Landed", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
    static UAbilityTask_WaitLanded* WaitLanded(UGameplayAbility* OwningAbility);

private:
    // �½�ũ�� Ȱ��ȭ�� �� ȣ��Ǵ� ���� �Լ��Դϴ�.
    virtual void Activate() override;
    // �½�ũ�� �Ҹ�� �� ȣ��Ǿ� ���ε��� �����մϴ�.
    virtual void OnDestroy(bool AbilityEnded) override;

    // ĳ������ OnLanded ��������Ʈ�� ������ ���ε��� �Լ��Դϴ�.
    UFUNCTION()
    void OnLandedCallback(const FHitResult& HitResult);

    ACharacter* GetTargetCharacter() const;
};
