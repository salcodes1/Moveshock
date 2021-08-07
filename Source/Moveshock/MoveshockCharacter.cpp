// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveshockCharacter.h"

#include "DrawDebugHelpers.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AMoveshockCharacter::AMoveshockCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	Camera->SetupAttachment(RootComponent);

	Camera->AddRelativeLocation(FVector(0.00f, 0.00f, 64.00f));
	Camera->SetFieldOfView(110.00f);
	
	Camera->bUsePawnControlRotation = true;
	
}

// Called when the game starts or when spawned
void AMoveshockCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->bApplyGravityWhileJumping = false;
	GetCharacterMovement()->Mass = 3.f;
}

void AMoveshockCharacter::Jump()
{
	bWantsToJump = true;
	Super::Jump();
}

void AMoveshockCharacter::StopJumping()
{
	bWantsToJump = false;
	Super::StopJumping();
}


// Called every frame
void AMoveshockCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<FHitResult> SphereHits;

	GetWorld()->SweepMultiByChannel(SphereHits, GetActorLocation(), GetActorLocation() + FVector(10), FQuat::Identity, ECC_WorldStatic, FCollisionShape::MakeSphere(200));
	
	static float lastWallrunTime = 0;
	
	FHitResult FinalHit;
		
	bIsWallrunning = false;

	for(FHitResult& SphereHit : SphereHits)
	{
		// if(!IsValid(SphereHit.GetComponent()))
		// 	GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Red, TEXT("BRUH"));
		// else
		// 	GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Red, SphereHit.GetActor()->GetName());
		if(IsValid(SphereHit.GetComponent()) && SphereHit.GetActor() != this)
		{
			if(GetCharacterMovement()->IsFalling())
			{
					bIsWallrunning = true;
					FinalHit = SphereHit;
			}
			break;
		}
	}
	if(bWantsToJump && bIsWallrunning)
	{
		bIsWallrunning = false;
		GetCharacterMovement()->AddImpulse((FinalHit.ImpactNormal + 1.2 * FVector::UpVector) * 200);
	}
	
	if(bIsWallrunning)
	{
		Camera->SetFieldOfView(115.f);
		GEngine->AddOnScreenDebugMessage(-1, 0.2f, FColor::Red, "ADDING");
		GetCharacterMovement()->AddForce(FVector::CrossProduct(FinalHit.ImpactNormal, FVector(0, 1, 0)) * 2);
		DrawDebugLine(GetWorld(), FinalHit.ImpactPoint, FinalHit.ImpactPoint + FVector::CrossProduct(FinalHit.ImpactNormal, FVector(0, 1, 0)) * 200, FColor::Cyan, false, 100);
		// DrawDebugLine(GetWorld(), FinalHit.ImpactPoint, (FinalHit.ImpactPoint + FinalHit.ImpactNormal).BackwardVector * 400, FColor::Red, false, 100);
		// DrawDebugLine(GetWorld(), FinalHit.Location, FinalHit.Location + FinalHit.ImpactNormal.BackwardVector * 200, FColor::Red, false, 100);
		// DrawDebugLine(GetWorld(), FinalHit.Location, FinalHit.Location + FinalHit.ImpactNormal.LeftVector * 400, FColor::Green, false, 100);
		if(GetVelocity().Z < 0)
			GetCharacterMovement()->AddForce(FVector(0, 0, -1 * GetVelocity().Z) * 100);
		lastWallrunTime = FPlatformTime::ToMilliseconds(FPlatformTime::Cycles());
	}
	else Camera->SetFieldOfView(110.f);
	
}

// Called to bind functionality to input
void AMoveshockCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("LookUp", this, &AMoveshockCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &AMoveshockCharacter::AddControllerYawInput);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMoveshockCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMoveshockCharacter::MoveRight);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMoveshockCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMoveshockCharacter::StopJumping);
	
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AMoveshockCharacter::StartCrouchSlide);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AMoveshockCharacter::StopCrouchSlide);
}


void AMoveshockCharacter::MoveForward(float Value)
{
	FVector Direction = FRotationMatrix(GetActorRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}
void AMoveshockCharacter::MoveRight(float Value)
{
	FVector Direction = FRotationMatrix(GetActorRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}

void AMoveshockCharacter::StartCrouchSlide()
{
	Crouch();
	Slide();
}

void AMoveshockCharacter::StopCrouchSlide()
{
	UnCrouch();
}

void AMoveshockCharacter::Slide()
{
	GetCharacterMovement()->AddImpulse(GetVelocity() * 5, true);
	GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, "HELOOOO");
}