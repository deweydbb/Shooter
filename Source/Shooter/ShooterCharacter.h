// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Core.h"
#include "MagazineLoad.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

class UInputComponent;

UCLASS(config=Game)
class AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	AShooterCharacter();

	MagazineLoad mag;

	class USkeletalMeshComponent* PlayerMesh;

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite)
		float health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float maxHealth;

	UFUNCTION()
		void removeHealth(AShooterCharacter* CharactHit);
	UFUNCTION()
		void addHealth(AShooterCharacter* CharactHit);

	UFUNCTION()
		int getHealth();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Magazine)
		bool ableToFire;


	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite)
		bool isDead;

	//variables used to display wigets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Magazine)
		float MaxBullets;

	//variables used to display wigets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Magazine)
		float CurrBullets;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Replicated, Category = Mesh)
		class USceneComponent* FP_MuzzleLocation;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	bool team;

	UFUNCTION()
	void spawnChar();

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Replicated, Category = Projectile)
		TSubclassOf<class ASniperProjectile> ProjectileClass;

	//ASniperProjectile* const Projectile = World->SpawnActor<ASniperProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);


	FVector gunOffset;
	FVector firePoint;

	UFUNCTION()
		void Fire();


	UFUNCTION(Server, WithValidation, Reliable)
		void ServerFire();
	UFUNCTION(NetMultiCast, Reliable)
		void OutwardFire();

	UFUNCTION(Server, WithValidation, Reliable)
		void ServerRemoveHealth(AShooterCharacter* CharactHit);

	UFUNCTION(Server, WithValidation, Reliable)
		void ServeraddHealth(AShooterCharacter* CharactHit);

	UFUNCTION(NetMultiCast, Reliable)
		void ClientAddHealth(AShooterCharacter* CharactHit);

	UFUNCTION(NetMultiCast, Reliable)
		void ClientRemoveHealth(AShooterCharacter* CharactHit);
protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	void Reload();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

