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
    // 블루프린트와 C++에서 호출할 수 있는 태스크의 출력 핀입니다.
    UPROPERTY(BlueprintAssignable)
    FWaitLandedDelegate OnLanded;

    // 이 태스크를 생성하고 활성화하는 정적 함수입니다. 블루프린트 노드의 생성 핀 역할을 합니다.
    UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "Wait For Landed", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
    static UAbilityTask_WaitLanded* WaitLanded(UGameplayAbility* OwningAbility);

private:
    // 태스크가 활성화될 때 호출되는 메인 함수입니다.
    virtual void Activate() override;
    // 태스크가 소멸될 때 호출되어 바인딩을 해제합니다.
    virtual void OnDestroy(bool AbilityEnded) override;

    // 캐릭터의 OnLanded 델리게이트에 실제로 바인딩될 함수입니다.
    UFUNCTION()
    void OnLandedCallback(const FHitResult& HitResult);

    ACharacter* GetTargetCharacter() const;
};
