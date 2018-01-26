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
		int playerID;

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite)
		FString nickName;

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite)
		FString teamName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString hitByLast;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString lastKilledBy;

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite)
		float health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float maxHealth;

	UFUNCTION()
		void removeHealth();
	UFUNCTION()
		void addHealth();

	UFUNCTION()
		int getHealth();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Magazine)
		bool ableToFire;


	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite)
		bool isDead;

	//variables used to display wigets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Magazine, Replicated)
		float MaxBullets;

	//variables used to display wigets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Magazine, Replicated)
		float CurrBullets;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Replicated, Category = Mesh)
		class USceneComponent* FP_MuzzleLocation;


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
		void ServerRemoveHealth();

	UFUNCTION(Server, WithValidation, Reliable)
		void ServeraddHealth();

	UFUNCTION(NetMultiCast, Reliable)
		void ClientAddHealth();

	UFUNCTION(NetMultiCast, Reliable)
		void ClientRemoveHealth();

	UPROPERTY(Replicated, BlueprintReadWrite)
		FRotator SpawnRotation;
protected:

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

