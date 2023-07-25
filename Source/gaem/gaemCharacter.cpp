// Copyright Epic Games, Inc. All Rights Reserved.

#include "gaemCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


//////////////////////////////////////////////////////////////////////////
// AgaemCharacter

AgaemCharacter::AgaemCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AgaemCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// Start with full dashes
	AvailableDashes = MaxAvailableDashes;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AgaemCharacter::Shoot(){

	FVector loc;
	FRotator rot;
	FHitResult hit;

	if (Controller) {
		Controller->GetPlayerViewPoint(loc, rot);

		FVector start = loc;
		FVector end = start + (rot.Vector() * 1500);

		FCollisionQueryParams collision;

		GetWorld()->LineTraceSingleByChannel(hit, start, end, ECC_Visibility, collision);

		auto actors = hit.GetActor();

		if (actors && actors->IsRootComponentMovable()) {
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Shot an actor"));

			actors->SetActorLocation(start);
		}
	}
}

void AgaemCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AgaemCharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AgaemCharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AgaemCharacter::Move);
		// Also call this to reset the buttons when we finish moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AgaemCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AgaemCharacter::Look);

		// Shooting
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &AgaemCharacter::Shoot);

		// Dashing
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &AgaemCharacter::StartDash);
	}
}

void AgaemCharacter::Jump()
{
	ACharacter::Jump();
	InputDirection.Z = 1.0f;
}

void AgaemCharacter::StopJumping()
{
	ACharacter::StopJumping();
	InputDirection.Z = 0.0f;
}

FRotator AgaemCharacter::GetYawRotation() {
	// find out which way is forward
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	return YawRotation;
}

void AgaemCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	InputDirection.Y = MovementVector.X;
	InputDirection.X = MovementVector.Y;

	if (Controller != nullptr)
	{
		// Only allow the player to move if not dashing
		if (!IsDashing()) {
			FRotator YawRotation = GetYawRotation();

			// get forward vector
			const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

			// get right vector 
			const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

			// add movement 
			AddMovementInput(ForwardDirection, MovementVector.Y);
			AddMovementInput(RightDirection, MovementVector.X);
		}
	}
}

void AgaemCharacter::StartDash() {
	// Do not dash again if already dashing
	if (IsDashing()) return;

	// If we have no more dashes, we can't dash
	if (AvailableDashes <= 0) return;

	// Get the desired dash direction
	DashVector = InputDirection.GetSafeNormal() * DashSpeed;

	// If no direction pressed, instead move forwards
	if (DashVector.IsZero()) DashVector.X = DashSpeed;
	
	// Finally, rotate the dash vector to bo global
	DashVector = GetYawRotation().RotateVector(DashVector);

	// If we removed the first dash, queue dash refreshing
	if (AvailableDashes == MaxAvailableDashes) 
		GetWorldTimerManager().SetTimer(DashRefreshTimerHandle, this, &AgaemCharacter::RefreshDash, DashRefreshTime, true);

	// Reduce the number of available dashes
	AvailableDashes -= 1;

	IsCoyoteTimeActive = false;

	GetWorldTimerManager().SetTimer(DashTimerHandle, this, &AgaemCharacter::StopDash, DashTime);
}

void AgaemCharacter::StopDash() {
	GetWorldTimerManager().ClearTimer(DashTimerHandle);
	DashVector = FVector::Zero();
	// Stop the character
	// TODO: remove this - this is jank so that velocity gets reset to almost 0
	// TODO: make the dashes more smooth
	// TODO: maybe add a grace period for button presses?
	// TODO: fix animations
	// TODO: set a custom movement mode
	this->LaunchCharacter(FVector(0.01f, 0.0f, 0.0f), true, true);
}

void AgaemCharacter::RefreshDash() {
	AvailableDashes++;

	if (AvailableDashes >= MaxAvailableDashes)
		GetWorldTimerManager().ClearTimer(DashRefreshTimerHandle);
}

void AgaemCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AgaemCharacter::Tick(float DeltaSeconds) {
	if (!IsDashing()) return;
	this->LaunchCharacter(DashVector, true, true);
}

bool AgaemCharacter::IsDashing() const {
	return !DashVector.IsZero();
}

bool AgaemCharacter::CanJumpInternal_Implementation() const {
	return IsCoyoteTimeActive || ACharacter::CanJumpInternal_Implementation();
}