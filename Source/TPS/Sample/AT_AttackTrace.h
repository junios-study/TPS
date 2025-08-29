// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AT_AttackTrace.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityTaskTargetData, const FGameplayAbilityTargetDataHandle&, TargetDataHandle);
/**
 * 
 */
UCLASS()
class TPS_API UAT_AttackTrace : public UAbilityTask
{
	GENERATED_BODY()

public:
	UAT_AttackTrace();

	virtual void Activate() override;
	virtual void OnDestroy(bool bInOwnerFinished) override;

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "AttackTrace", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "TRUE"))
	static UAT_AttackTrace* CreateTask(UGameplayAbility* OwingAbility, TSubclassOf<class AGATA_AttackTrace> TargetActorClass);

	TSubclassOf<class AGATA_AttackTrace> TargetActorClass;

	UFUNCTION()
	void CompleteTargetDataReady(const FGameplayAbilityTargetDataHandle& Handle);

	UPROPERTY(BlueprintAssignable)
	FAbilityTaskTargetData OnCompleted;
	
};
