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