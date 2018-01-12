// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Core.h"
#include "GameFramework/Actor.h"
#include "SniperProjectile.generated.h"

UCLASS(config=Game)
class ASniperProjectile : public AActor
{
	GENERATED_BODY()


	/**Sphere collsion component */

public:
	UPROPERTY(VisibleDefaultsOnly, Replicated, Category = Projectile)
	class USphereComponent* CollisionComp;

	/**Projectile movement component*/
	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement;
public:	
	// Sets default values for this actor's properties
	ASniperProjectile();

	void Fire();

	bool once;

	UPROPERTY(Replicated)
	int playerOwnerID;

	UPROPERTY(Replicated)
		FString playerOwnerTeam;
	
	/**called when projectile collides*/
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Returns CollisionComp subobject **/
	FORCEINLINE class USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
	
};
