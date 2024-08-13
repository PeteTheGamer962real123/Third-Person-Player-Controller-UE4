// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ThirdPersonPlayer.generated.h"

UCLASS()
class CPPTHIRDPERSON_API AThirdPersonPlayer : public ACharacter
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComp;

public:
	// Sets default values for this character's properties
	AThirdPersonPlayer();

	bool bIsSprinting = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	float ForwardValue = 0.f;
	void MoveRight(float Value);
	float RightValue = 0.f;

	void StartSprint();
	void StopSprint();

	void StartCrouch();
	void StopCrouch();

	void Interact();
	UPROPERTY(EditAnywhere)
	float InteractLength = 350.f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
