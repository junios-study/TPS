// Copyright Epic Games, Inc. All Rights Reserved.

#include "TPSCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "TPS.h"
#include "Sample/TPSPlayerState.h"
#include "AbilitySystemComponent.h"
#include "Sample/GameplayAbility_ComboAttack.h"

ATPSCharacter::ATPSCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void ATPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		//EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATPSCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &ATPSCharacter::Look);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATPSCharacter::Look);

		SetupGASInputComponent();

	}
	else
	{
		UE_LOG(LogTPS, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ATPSCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void ATPSCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void ATPSCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void ATPSCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void ATPSCharacter::DoJumpStart()
{
	// signal the character to jump
	Jump();
}

void ATPSCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}

void ATPSCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ATPSPlayerState* PS = GetPlayerState<ATPSPlayerState>();

	if (IsValid(PS))
	{
		ASC = PS->GetAbilitySystemComponent();
		if (IsValid(ASC))
		{
			ASC->InitAbilityActorInfo(PS, this);
			ASC->SetIsReplicated(true);

			for (const auto& AbilityClass : GiveAbilities)
			{
				FGameplayAbilitySpec Spec(AbilityClass);
				ASC->GiveAbility(Spec);
			}

			for (const auto& InputAbilityClass : InputAbilities)
			{
				FGameplayAbilitySpec Spec(InputAbilityClass.Value);
				Spec.InputID = InputAbilityClass.Key;
				ASC->GiveAbility(Spec);
			}

			APlayerController* PC = Cast<APlayerController>(NewController);
			PC->ConsoleCommand(TEXT("ShowDebug AbilitySystem"), true);
		}
	}

}

void ATPSCharacter::SetupGASInputComponent()
{

	if (IsValid(ASC) && IsValid(InputComponent))
	{
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
		{
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ATPSCharacter::InputPressed, 0);
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ATPSCharacter::InputReleased, 0);

			EnhancedInputComponent->BindAction(ComboAttackAction, ETriggerEvent::Triggered, this, &ATPSCharacter::InputPressed, 1);
			EnhancedInputComponent->BindAction(ComboAttackAction, ETriggerEvent::Triggered, this, &ATPSCharacter::InputReleased, 1);
		}
	}
}

void ATPSCharacter::InputPressed(int32 InputID)
{
	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputID);
	if (Spec)
	{
		if (Spec->IsActive())
		{
			ASC->AbilitySpecInputPressed(*Spec);
		}
		else
		{
			ASC->TryActivateAbility(Spec->Handle);
		}
	}
}

void ATPSCharacter::InputReleased(int32 InputID)
{
	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(InputID);
	if (Spec)
	{
		Spec->InputPressed = false;
		if (Spec->IsActive())
		{
			ASC->AbilitySpecInputReleased(*Spec);
		}
	}
}

void ATPSCharacter::DoAttackTrace(FName DamageSourceBone)
{
	// sweep for objects in front of the character to be hit by the attack
	TArray<FHitResult> OutHits;

	// start at the provided socket location, sweep forward
	const FVector TraceStart = GetMesh()->GetSocketLocation(DamageSourceBone);
	const FVector TraceEnd = TraceStart + (GetActorForwardVector() * MeleeTraceDistance);

	// check for pawn and world dynamic collision object types
	FCollisionObjectQueryParams ObjectParams;
	ObjectParams.AddObjectTypesToQuery(ECC_Pawn);
	ObjectParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	// use a sphere shape for the sweep
	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(MeleeTraceRadius);

	// ignore self
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	if (GetWorld()->SweepMultiByObjectType(OutHits, TraceStart, TraceEnd, FQuat::Identity, ObjectParams, CollisionShape, QueryParams))
	{
		// iterate over each object hit
		for (const FHitResult& CurrentHit : OutHits)
		{
			// check if we've hit a damageable actor
			ICombatDamageable* Damageable = Cast<ICombatDamageable>(CurrentHit.GetActor());

			if (Damageable)
			{
				// knock upwards and away from the impact normal
				const FVector Impulse = (CurrentHit.ImpactNormal * -MeleeKnockbackImpulse) + (FVector::UpVector * MeleeLaunchImpulse);

				// pass the damage event to the actor
				Damageable->ApplyDamage(MeleeDamage, this, CurrentHit.ImpactPoint, Impulse);

				// call the BP handler to play effects, etc.
//				DealtDamage(MeleeDamage, CurrentHit.ImpactPoint);
			}
		}
	}
}

//AnimNotify가 실행 시켜줌(ICombatAttacker)
void ATPSCharacter::CheckCombo()
{
	if (bIsAttacking)
	{
		if (GetWorld()->GetTimeSeconds() - CachedAttackInputTime <= ComboInputCacheTimeTolerance)
		{
			CachedAttackInputTime = 0.0f;
			++ComboCount;

			if (ComboCount < ComboSectionNames.Num())
			{
				//GameplayAbility_ComboAttack, 아래 기능 추가(CheckCombo)
				//GameplayAbility_ComboAttack 이미 생성 된거임(전제), 입력 했으니깐
				//현재 실행 중인 GameplayAbility_ComboAttack 인스턴스를 찾아서 호출 해줘야 됨

				FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromInputID(1);
				for (UGameplayAbility* Instance : Spec->GetAbilityInstances())
				{
					UGameplayAbility_ComboAttack* Ability = Cast<UGameplayAbility_ComboAttack>(Instance);
					if (IsValid(Ability))
					{
						Ability->JumpToSection();
					}
				}
			}
		}
	}
}

void ATPSCharacter::CheckChargedAttack()
{
}
