// Fill out your copyright notice in the Description page of Project Settings.


#include "ThirdPersonPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <DrawDebugHelpers.h>

// Sets default values
AThirdPersonPlayer::AThirdPersonPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->bEnableCameraLag = true;
	SpringArmComp->TargetArmLength = 200.f;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bIgnoreBaseRotation = true;
}

// Called when the game starts or when spawned
void AThirdPersonPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

void AThirdPersonPlayer::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector() * Value);
	ForwardValue = Value;
}

void AThirdPersonPlayer::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector() * Value);
	RightValue = Value;
}

void AThirdPersonPlayer::StartSprint()
{
	bIsSprinting = true;
}

void AThirdPersonPlayer::StopSprint()
{
	bIsSprinting = false;
}

void AThirdPersonPlayer::StartCrouch()
{
	Crouch();
}

void AThirdPersonPlayer::StopCrouch()
{
	UnCrouch();
}

void AThirdPersonPlayer::Interact()
{
	if (SpringArmComp == nullptr) return;

	FHitResult HitResult;
	FVector Start = SpringArmComp->GetComponentLocation();
	FVector End = Start + SpringArmComp->GetForwardVector() * InteractLength;
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility);
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.f);
	DrawDebugPoint(GetWorld(), End, 20.f, FColor::Red, false, 2.f);
	DrawDebugPoint(GetWorld(), Start, 20.f, FColor::Blue, false, 2.f);
}

// Called every frame
void AThirdPersonPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ForwardValue > 0 && bIsSprinting)
		GetCharacterMovement()->MaxWalkSpeed = 600.f;
	else if (ForwardValue < 0 || !bIsSprinting)
		GetCharacterMovement()->MaxWalkSpeed = 180.f;
	else if (RightValue > 0)
		GetCharacterMovement()->MaxWalkSpeed = 180.f;
	else if (RightValue < 0)
		GetCharacterMovement()->MaxWalkSpeed = 180.f;
}

// Called to bind functionality to input
void AThirdPersonPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AThirdPersonPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AThirdPersonPlayer::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &AThirdPersonPlayer::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AThirdPersonPlayer::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AThirdPersonPlayer::StartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AThirdPersonPlayer::StopSprint);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AThirdPersonPlayer::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AThirdPersonPlayer::StopJumping);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AThirdPersonPlayer::StartCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AThirdPersonPlayer::StopCrouch);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AThirdPersonPlayer::Interact);
}