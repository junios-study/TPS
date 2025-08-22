// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "TPSPlayerState.generated.h"

class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class TPS_API ATPSPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	ATPSPlayerState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UPROPERTY(VisibleAnywhere, Category=Component)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
};
