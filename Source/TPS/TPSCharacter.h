// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CombatAttacker.h"
#include "CombatDamageable.h"
#include "Logging/LogMacros.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemInterface.h"
#include "TPSCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;
class UGameplayAbility;
class UAbilitySystemComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

/**
 *  A simple player-controllable third person character
 *  Implements a controllable orbiting camera
 */
UCLASS(abstract)
class ATPSCharacter : public ACharacter, public ICombatAttacker, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
protected:

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MouseLookAction;

	/** Combo Attack Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* ComboAttackAction;

public:

	/** Constructor */
	ATPSCharacter();	

	/** AnimMontage that will play for combo attacks */
	UPROPERTY(EditAnywhere, Category = "Melee Attack|Combo")
	UAnimMontage* ComboAttackMontage;

protected:

	/** Initialize input action bindings */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

public:

	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles look inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoLook(float Yaw, float Pitch);

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();

public:

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }


	virtual void PossessedBy(AController* NewController) override;

	UPROPERTY(EditAnywhere, Category = Ability)
	TArray<TSubclassOf<UGameplayAbility>> GiveAbilities;

	UPROPERTY(EditAnywhere, Category = Ability)
	TMap<int32, TSubclassOf<UGameplayAbility>> InputAbilities;

	void SetupGASInputComponent();

	void InputPressed(int32 InputID);

	void InputReleased(int32 InputID);


	UPROPERTY(VisibleAnywhere, Category="Components")
	UAbilitySystemComponent* ASC;


	/** Performs an attack's collision check. Usually called from a montage's AnimNotify */
	UFUNCTION(BlueprintCallable, Category = "Attacker")
	virtual void DoAttackTrace(FName DamageSourceBone) override;

	/** Performs a combo attack's check to continue the string. Usually called from a montage's AnimNotify */
	UFUNCTION(BlueprintCallable, Category = "Attacker")
	virtual void CheckCombo() override;

	/** Performs a charged attack's check to loop the charge animation. Usually called from a montage's AnimNotify */
	UFUNCTION(BlueprintCallable, Category = "Attacker")
	virtual void CheckChargedAttack() override;


	/** Max amount of time that may elapse for a non-combo attack input to not be considered stale */
	UPROPERTY(EditAnywhere, Category = "Melee Attack", meta = (ClampMin = 0, ClampMax = 5, Units = "s"))
	float AttackInputCacheTimeTolerance = 1.0f;

	/** Time at which an attack button was last pressed */
	float CachedAttackInputTime = 0.0f;

	/** If true, the character is currently playing an attack animation */
	bool bIsAttacking = false;

	/** Distance ahead of the character that melee attack sphere collision traces will extend */
	UPROPERTY(EditAnywhere, Category = "Melee Attack|Trace", meta = (ClampMin = 0, ClampMax = 500, Units = "cm"))
	float MeleeTraceDistance = 75.0f;

	/** Radius of the sphere trace for melee attacks */
	UPROPERTY(EditAnywhere, Category = "Melee Attack|Trace", meta = (ClampMin = 0, ClampMax = 200, Units = "cm"))
	float MeleeTraceRadius = 75.0f;

	/** Amount of damage a melee attack will deal */
	UPROPERTY(EditAnywhere, Category = "Melee Attack|Damage", meta = (ClampMin = 0, ClampMax = 100))
	float MeleeDamage = 1.0f;

	/** Amount of knockback impulse a melee attack will apply */
	UPROPERTY(EditAnywhere, Category = "Melee Attack|Damage", meta = (ClampMin = 0, ClampMax = 1000, Units = "cm/s"))
	float MeleeKnockbackImpulse = 250.0f;

	/** Amount of upwards impulse a melee attack will apply */
	UPROPERTY(EditAnywhere, Category = "Melee Attack|Damage", meta = (ClampMin = 0, ClampMax = 1000, Units = "cm/s"))
	float MeleeLaunchImpulse = 300.0f;

	/** Names of the AnimMontage sections that correspond to each stage of the combo attack */
	UPROPERTY(EditAnywhere, Category = "Melee Attack|Combo")
	TArray<FName> ComboSectionNames;

	/** Max amount of time that may elapse for a combo attack input to not be considered stale */
	UPROPERTY(EditAnywhere, Category = "Melee Attack|Combo", meta = (ClampMin = 0, ClampMax = 5, Units = "s"))
	float ComboInputCacheTimeTolerance = 0.45f;

	/** Index of the current stage of the melee attack combo */
	int32 ComboCount = 0;

	FName DamageSourceBone;

	UPROPERTY(EditAnywhere, Category="Ability|AttackTrace")
	FGameplayTag AttackTraceTag;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

};

