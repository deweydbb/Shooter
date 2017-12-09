// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ShooterCharacter.h"
#include "MagazineLoad.h"
#include "SniperProjectile.h"
#include "Net/UnrealNetwork.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Actor.h"
#include "GameFramework/SpringArmComponent.h"

//////////////////////////////////////////////////////////////////////////
// AShooterCharacter

AShooterCharacter::AShooterCharacter()
{
	bReplicates = true;

	PlayerMesh = GetMesh();
	mag.BulletsLeft = mag.TotalSize;
	isDead = false;
	maxHealth = 100;
	health = maxHealth;
	//used for wiget display
	CurrBullets = mag.BulletsLeft;
	MaxBullets = mag.TotalSize;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;


	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FollowCamera);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	gunOffset = FVector(133.4f, 24.45f, 42.75);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;



	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	team = true;
}

int AShooterCharacter::getHealth()
{
	return health;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AShooterCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AShooterCharacter::Reload);

	//fire action
		PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AShooterCharacter::Fire);

	//PlayerInputComponent->BindAxis("MoveForward", this, &AShooterCharacter::MoveForward);
	//PlayerInputComponent->BindAxis("MoveRight", this, &AShooterCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AShooterCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AShooterCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AShooterCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AShooterCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AShooterCharacter::OnResetVR);
}


void AShooterCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AShooterCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AShooterCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AShooterCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

/*void AShooterCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AShooterCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
} */

void AShooterCharacter::spawnChar() {

}

void AShooterCharacter::Fire()
{
	if (Role < ROLE_Authority) {
		ServerFire();
	}
	else {
		OutwardFire();
	}
}

void AShooterCharacter::ServerFire_Implementation()
{
	if (mag.CanFire() && ableToFire) {

		if (ProjectileClass != NULL)
		{
			mag.FireBullet();
			if (CurrBullets > 0) {
				CurrBullets -= 1;
			}


			UWorld* const World = GetWorld();
			if (World != NULL)
			{
				const FRotator SpawnRotation = GetControlRotation();
				// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
				//const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(gunOffset);
				PlayerMesh->GetSocketLocation("Muzzle");
				gunOffset = PlayerMesh->GetSocketLocation("Muzzle") + FVector(100.f, 0.f, 0.f);

				//Set Spawn Collision Handling Override
				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

				// spawn the projectile at the muzzle
				UPROPERTY(replicated)
					ASniperProjectile* Projectile = World->SpawnActor<ASniperProjectile>(ProjectileClass, gunOffset, SpawnRotation, ActorSpawnParams);
			}
		}
	}

	OutwardFire();
}

bool AShooterCharacter::ServerFire_Validate() {
	return true;
}

void AShooterCharacter::OutwardFire_Implementation()
{
	if (mag.CanFire() && ableToFire) {

		if (ProjectileClass != NULL)
		{
			mag.FireBullet();
			if (CurrBullets > 0) {
				CurrBullets -= 1;
			}


			UWorld* const World = GetWorld();
			if (World != NULL)
			{
				const FRotator SpawnRotation = GetControlRotation();
				// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
				//const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(gunOffset);
				PlayerMesh->GetSocketLocation("Muzzle");
				gunOffset = PlayerMesh->GetSocketLocation("Muzzle");

				//Set Spawn Collision Handling Override
				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

				// spawn the projectile at the muzzle
				UPROPERTY(replicated)
					ASniperProjectile* Projectile = World->SpawnActor<ASniperProjectile>(ProjectileClass, gunOffset, SpawnRotation);
			}
		}
	}
}

void AShooterCharacter::removeHealth(AShooterCharacter* CharactHit) {
	//if (Role < ROLE_Authority) {
		ServerRemoveHealth(CharactHit);
	//}
	//else {
	//	ClientRemoveHealth(CharactHit);
	//}
}

void AShooterCharacter::addHealth(AShooterCharacter* CharactHit) {
	//if (Role < ROLE_Authority) {
	//	ServeraddHealth(CharactHit);
	//}
	//else {
		ClientAddHealth(CharactHit);
	//}
}

void AShooterCharacter::ServerRemoveHealth_Implementation(AShooterCharacter* CharactHit) {
	health -= 33/2;
	ClientRemoveHealth(CharactHit);
}

bool AShooterCharacter::ServerRemoveHealth_Validate(AShooterCharacter* CharactHit) {
	return true;
}

void AShooterCharacter::ServeraddHealth_Implementation(AShooterCharacter* CharactHit) {
	health += 33/2;
	ClientAddHealth(CharactHit);
}

bool AShooterCharacter::ServeraddHealth_Validate(AShooterCharacter* CharactHit) {
	return true;
}

void AShooterCharacter::ClientAddHealth_Implementation(AShooterCharacter* CharactHit) {
	health += 33/2;
}

void AShooterCharacter::ClientRemoveHealth_Implementation(AShooterCharacter* CharactHit) {
	health -= 33/2;
}


void AShooterCharacter::Reload()
{
	mag.ReloadMagazine();
	CurrBullets = mag.TotalSize;
}

void AShooterCharacter::GetLifetimeReplicatedProps(TArray < FLifetimeProperty > & OutLifetimeProps) const 
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AShooterCharacter, health);
	DOREPLIFETIME(AShooterCharacter, isDead);
	DOREPLIFETIME(AShooterCharacter, FP_MuzzleLocation);
	DOREPLIFETIME(AShooterCharacter, ProjectileClass);
}