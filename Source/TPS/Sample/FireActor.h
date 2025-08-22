// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "FireActor.generated.h"

class UAbilitySystemComponent;
class UGameplayAbility;

UCLASS()
class TPS_API AFireActor : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFireActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UPROPERTY(VisibleAnywhere, Category = Ability)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditAnywhere, Category = Ability)
	TArray<TSubclassOf<UGameplayAbility>> GiveAbilities;

	UPROPERTY(EditAnywhere, Category = "Ability | Tags")
	FGameplayTagContainer AbilityTagName;

	UPROPERTY(EditAnywhere, Category = "Ability | Tags")
	FGameplayTag EffctAbilityTagName;
};
