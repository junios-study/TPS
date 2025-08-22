// Fill out your copyright notice in the Description page of Project Settings.


#include "Sample/TPSPlayerState.h"
#include "AbilitySystemComponent.h"

ATPSPlayerState::ATPSPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
}

UAbilitySystemComponent* ATPSPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
