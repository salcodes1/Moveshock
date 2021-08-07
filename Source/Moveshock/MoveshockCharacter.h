// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"

#include "MoveshockCharacter.generated.h"

UCLASS()
class MOVESHOCK_API AMoveshockCharacter : public ACharacter
{
	GENERATED_BODY()

	
// Members
public:

	UPROPERTY()
	bool bWantsToJump;
	
	UPROPERTY()
	UCameraComponent* Camera;

	UPROPERTY()
	bool bIsWallrunning;
	
// Methods
public:
	// Sets default values for this character's properties
	AMoveshockCharacter();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Jump() override;
	virtual void StopJumping() override;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float Value);
	void MoveRight(float Value);

	void StartCrouchSlide();
	void StopCrouchSlide();
	void Slide();
	
	
};
