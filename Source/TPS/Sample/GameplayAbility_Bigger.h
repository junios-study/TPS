// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayAbility_Bigger.generated.h"

/**
 * 
 */
UCLASS()
class TPS_API UGameplayAbility_Bigger : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGameplayAbility_Bigger();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
	
public:
	UFUNCTION(BlueprintCallable)
	void OnCompleted();
};
