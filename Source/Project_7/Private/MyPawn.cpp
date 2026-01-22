#include "MyPawn.h"
#include "PawnController.h"
#include "EnhancedInputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"

AMyPawn::AMyPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	RootComponent = CapsuleComp;
	CapsuleComp->InitCapsuleSize(42.f, 96.f);

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetRelativeLocation(FVector(0.f, 0.f, -CapsuleComp->GetUnscaledCapsuleHalfHeight()));
	MeshComp->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 300.f;
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->bEnableCameraLag = true;
	SpringArmComp->CameraLagSpeed = 10.f;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
	CameraComp->bUsePawnControlRotation = false;

	AutoPossessPlayer = EAutoReceiveInput::Player0;
	bIsJumping = false;
	Speed = 0.f;
	LastMoveInput = FVector2D::ZeroVector;
}

void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (APawnController* PC = Cast<APawnController>(GetController()))
		{
			EnhancedInput->BindAction(PC->MoveAction, ETriggerEvent::Triggered, this, &AMyPawn::Move);
			EnhancedInput->BindAction(PC->MoveAction, ETriggerEvent::Completed, this, &AMyPawn::StopMove);

			EnhancedInput->BindAction(PC->LookAction, ETriggerEvent::Triggered, this, &AMyPawn::Look);

			EnhancedInput->BindAction(PC->JumpAction, ETriggerEvent::Triggered, this, &AMyPawn::StartJump);
			EnhancedInput->BindAction(PC->JumpAction, ETriggerEvent::Completed, this, &AMyPawn::StopJump);
		}
	}
}

void AMyPawn::Move(const FInputActionValue& Value)
{
	LastMoveInput = Value.Get<FVector2D>();
}

void AMyPawn::StopMove(const FInputActionValue& Value)
{
	LastMoveInput = FVector2D::ZeroVector;
}

void AMyPawn::Look(const FInputActionValue& Value)
{
	FVector2D LookInput = Value.Get<FVector2D>();
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (!FMath::IsNearlyZero(LookInput.X))
			PC->AddYawInput(LookInput.X);

		if (!FMath::IsNearlyZero(LookInput.Y))
			PC->AddPitchInput(LookInput.Y);
	}
}

void AMyPawn::StartJump(const FInputActionValue& Value)
{
	if (!bIsJumping)
	{
		bIsJumping = true;
		VerticalVelocity = JumpPower;
	}
}

void AMyPawn::StopJump(const FInputActionValue& Value)
{
}

void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		FRotator ControlRot = PC->GetControlRotation();
		FVector Forward = FRotationMatrix(FRotator(0.f, ControlRot.Yaw, 0.f)).GetUnitAxis(EAxis::X);
		FVector Right = FRotationMatrix(FRotator(0.f, ControlRot.Yaw, 0.f)).GetUnitAxis(EAxis::Y);

		FVector MoveDir = (Forward * LastMoveInput.Y + Right * LastMoveInput.X).GetSafeNormal();

		if (!MoveDir.IsNearlyZero())
		{
			FVector Delta = MoveDir * MoveSpeed * DeltaTime;
			AddActorWorldOffset(Delta, true);

			FRotator TargetRot = MoveDir.Rotation();
			FRotator NewRot = FMath::RInterpTo(GetActorRotation(), TargetRot, DeltaTime, TurnSpeed);
			SetActorRotation(FRotator(0.f, NewRot.Yaw, 0.f));
		}

		CurrentVelocity = MoveDir * MoveSpeed;
		Speed = CurrentVelocity.Size2D();
	}

	// 점프/중력
	FVector NewLocation = GetActorLocation();
	if (bIsJumping)
	{
		VerticalVelocity += Gravity * DeltaTime;
		NewLocation.Z += VerticalVelocity * DeltaTime;
	}

	float MinZ = CapsuleComp->GetUnscaledCapsuleHalfHeight();
	if (NewLocation.Z < MinZ)
	{
		NewLocation.Z = MinZ;
		bIsJumping = false;
		VerticalVelocity = 0.f;
	}

	SetActorLocation(NewLocation, true);
}
