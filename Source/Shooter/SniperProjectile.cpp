// Fill out your copyright notice in the Description page of Project Settings.
#include "SniperProjectile.h"
#include "Shooter.h"
#include "ShooterCharacter.h"
#include "MagazineLoad.h"
#include "Net/UnrealNetwork.h"
#include "Runtime/Engine/Classes/Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
ASniperProjectile::ASniperProjectile()
{

	// Use a sphere as a simple collision representation
	bReplicates = true;
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &ASniperProjectile::OnHit);		// set up a notification for when this component hits something blocking

																					// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 4000.f;
	ProjectileMovement->MaxSpeed = 5000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}


void ASniperProjectile::OnHit(UPrimitiveComponent * HitComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

		Destroy();
	}

	if (OtherActor->IsA<AShooterCharacter>()) {
		AShooterCharacter* HitMan = Cast<AShooterCharacter>(OtherActor);
		Destroy();
		HitMan->removeHealth();

		if (HitMan->getHealth() < 0) {
			//HitMan->Destroy();
		}

	}
	//Destroy();
}

void ASniperProjectile::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASniperProjectile, CollisionComp);
	DOREPLIFETIME(ASniperProjectile, ProjectileMovement);

}


