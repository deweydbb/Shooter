// Fill out your copyright notice in the Description page of Project Settings.
#include "SniperProjectile.h"
#include "Shooter.h"
#include "ShooterCharacter.h"
#include "MagazineLoad.h"
#include <string>
#include "Net/UnrealNetwork.h"
#include "Runtime/Engine/Classes/Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Runtime/Core/Public/Containers/UnrealString.h"


// Sets default values
ASniperProjectile::ASniperProjectile()
{

	// Use a sphere as a simple collision representation
	bReplicates = true;
	once = false;
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
	ProjectileMovement->InitialSpeed = 7000.f;
	ProjectileMovement->MaxSpeed = 8000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
	playerOwnerName = "";

	//team/ids
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
		//AShooterCharacter* OwnerPlayer = Cast<AShooterCharacter>(GetInstigator());

		UE_LOG(LogTemp, Warning, TEXT("is same team: %s"), (FString(playerOwnerTeam).Equals(FString(HitMan->teamName)) ? TEXT("True") : TEXT("False")));

		if (playerOwnerID == HitMan->playerID || FString(playerOwnerTeam).Equals(FString(HitMan->teamName))) {
			if (playerOwnerTeam.Equals("Free") && playerOwnerID != HitMan->playerID) {
				HitMan->removeHealth();
				HitMan->hitByLast = playerOwnerName;
				//AShooterCharacter * Player = get(AShooterCharacter, id: playerOwnerID);
				//GetInstigator()->playSoundOnHit();
				Destroy();
			}
		}
		else {
			HitMan->removeHealth();
			HitMan->hitByLast = playerOwnerName;
			//OwnerPlayer->playSoundOnHit();
			Destroy();
		}

	}
	Destroy();
}


void ASniperProjectile::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASniperProjectile, CollisionComp);
	DOREPLIFETIME(ASniperProjectile, ProjectileMovement);
	DOREPLIFETIME(ASniperProjectile, playerOwnerID);
	DOREPLIFETIME(ASniperProjectile, playerOwnerTeam);
	DOREPLIFETIME(ASniperProjectile, playerOwnerName);

}


