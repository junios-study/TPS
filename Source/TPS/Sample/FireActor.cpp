// Fill out your copyright notice in the Description page of Project Settings.


#include "Sample/FireActor.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbility_Bigger.h"
#include "GameplayAbilitySpec.h"

// Sets default values
AFireActor::AFireActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

}

// Called when the game starts or when spawned
void AFireActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsValid(AbilitySystemComponent))
	{
		//AbilitySystemComponent->HasMatchingGameplayTag(EffctAbilityTagName)
		AbilitySystemComponent->TryActivateAbilitiesByTag(AbilityTagName);
	}
}

void AFireActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(UGameplayAbility_Bigger::StaticClass()));

	if (IsValid(AbilitySystemComponent))
	{
		for (const auto& AbilityClass : GiveAbilities)
		{
			FGameplayAbilitySpec Spec(AbilityClass);
			AbilitySystemComponent->GiveAbility(Spec);
		}
	}
}

// Called every frame
void AFireActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UAbilitySystemComponent* AFireActor::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

