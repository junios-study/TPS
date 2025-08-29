// Fill out your copyright notice in the Description page of Project Settings.


#include "Sample/GATA_AttackTrace.h"
#include "Abilities/GameplayAbility.h"
#include "TPSCharacter.h"

AGATA_AttackTrace::AGATA_AttackTrace()
{
}

void AGATA_AttackTrace::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);

	SourceActor = Ability->GetCurrentActorInfo()->AvatarActor.Get();
}

void AGATA_AttackTrace::ConfirmTargetingAndContinue()
{
	if (SourceActor)
	{
		ATPSCharacter* Character = Cast<ATPSCharacter>(SourceActor);

		if (Character)
		{
			TArray<FHitResult> OutHits;

			const FVector TraceStart = Character->GetMesh()->GetSocketLocation(Character->DamageSourceBone);
			const FVector TraceEnd = TraceStart + (GetActorForwardVector() * Character->MeleeTraceDistance);

			FCollisionObjectQueryParams ObjectParams;
			ObjectParams.AddObjectTypesToQuery(ECC_Pawn);
			ObjectParams.AddObjectTypesToQuery(ECC_WorldDynamic);

			FCollisionShape CollisionShape;
			CollisionShape.SetSphere(Character->MeleeTraceRadius);

			FCollisionQueryParams QueryParams;
			QueryParams.AddIgnoredActor(Character);

			bool bHitCheck = GetWorld()->SweepMultiByObjectType(OutHits, TraceStart, TraceEnd, FQuat::Identity, ObjectParams, CollisionShape, QueryParams);

			DrawDebugSphere(GetWorld(),
				TraceStart,
				Character->MeleeTraceDistance / 2.f,
				10,
				bHitCheck ? FColor::Red : FColor::Green,
				false,
				2.0f,
				0,
				0.3f);

			FGameplayAbilityTargetDataHandle Handle;
			if (bHitCheck)
			{
				for (const FHitResult& CurrentHit : OutHits)
				{
					FGameplayAbilityTargetData_SingleTargetHit* TargetData = new FGameplayAbilityTargetData_SingleTargetHit(CurrentHit);
					Handle.Add(TargetData);
				}
			}

			TargetDataReadyDelegate.Broadcast(Handle);
		}
	}
}
