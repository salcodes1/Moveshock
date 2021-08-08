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

	GetCharacterMovement()->bApplyGravityWhileJumping = true;
	GetCharacterMovement()->JumpZVelocity = 470.f;
	GetCharacterMovement()->AirControl = 1.3f;
	GetCharacterMovement()->GravityScale = 1.3f;
	GetCharacterMovement()->Mass = 3.f;
	GetCharacterMovement()->SetWalkableFloorAngle(53.f);
	
	Super::JumpMaxCount = 2; // double jump
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

void AMoveshockCharacter::CheckForWallclimb(float DeltaTime)
{
	FHitResult WallclimbResult;

	FCollisionQueryParams QueryParams = {};
	QueryParams.AddIgnoredActor(this);

	const bool bHit = GetWorld()->LineTraceSingleByChannel(WallclimbResult, GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 100.f, ECC_WorldStatic, QueryParams);

	static float ClimbForce = 4000.f;
	static bool addedImpulse = false;
	static bool wallclimbReset = true;

	FVector2D wallNormal = FVector2D(WallclimbResult.ImpactNormal);

	FVector2D cameraNormal = FVector2D(Camera->GetForwardVector());
	
	float povAngle = 180 - FMath::Acos(FVector2D::DotProduct(wallNormal, cameraNormal) / wallNormal.Size() / cameraNormal.Size()) * 180 / PI;

	if(GetCharacterMovement()->IsFalling())
	{
		if(bHit && GetInputAxisValue("MoveForward") > 0.f && povAngle <= MaxWallclimbAngle && wallclimbReset)
		{
			if(!addedImpulse)
			{
				GetCharacterMovement()->Velocity.Z = 0;
				GetCharacterMovement()->AddImpulse(FVector(0, 0, GetCharacterMovement()->JumpZVelocity), true);
				addedImpulse = true;
				JumpCurrentCount = 0;
				GEngine->AddOnScreenDebugMessage(2, 0.1, FColor::Red, TEXT("IMPULSE"));
			}
			GetCharacterMovement()->AddForce(FVector(0, 0, ClimbForce));
			ClimbForce -= 150.f * DeltaTime;
			Camera->SetFieldOfView(FMath::FInterpTo(Camera->FieldOfView, 120.f, DeltaTime, 10.f));
		}
		
	}
	else
	{
		wallclimbReset = false;
		Camera->SetFieldOfView(FMath::FInterpTo(Camera->FieldOfView, 110.f, DeltaTime, 10.f));
		ClimbForce = 4000.f, addedImpulse = false;
	}
	if(!GetCharacterMovement()->IsFalling())
		wallclimbReset = true;
}


// Called every frame
void AMoveshockCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	CheckForWallclimb(DeltaTime);
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

}