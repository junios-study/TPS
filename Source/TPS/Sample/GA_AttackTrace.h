// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_AttackTrace.generated.h"

/**
 * 
 */
UCLASS()
class TPS_API UGA_AttackTrace : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGA_AttackTrace();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
};
