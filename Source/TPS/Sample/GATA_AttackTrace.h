// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "GATA_AttackTrace.generated.h"

/**
 * 
 */
UCLASS()
class TPS_API AGATA_AttackTrace : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()

public:
	AGATA_AttackTrace();

	
	virtual void StartTargeting(UGameplayAbility* Ability) override;

	virtual void ConfirmTargetingAndContinue() override;

	
};
