#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyPawn.generated.h"

class UCapsuleComponent;
class USkeletalMeshComponent;
class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;

UCLASS()
class PROJECT_7_API AMyPawn : public APawn
{
	GENERATED_BODY()

public:
	AMyPawn();

protected:
	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* CapsuleComp;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MoveSpeed = 400.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float TurnSpeed = 10.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float JumpPower = 600.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float Gravity = -980.f;

	UPROPERTY(BlueprintReadOnly, Category = "Anim")
	FVector CurrentVelocity;

	UPROPERTY(BlueprintReadOnly, Category = "Anim")
	float Speed;

	UPROPERTY(BlueprintReadOnly, Category = "Anim")
	bool bIsJumping;

private:
	FVector2D LastMoveInput;
	float VerticalVelocity = 0.f;

public:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;

	void Move(const FInputActionValue& Value);
	void StopMove(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void StartJump(const FInputActionValue& Value);
	void StopJump(const FInputActionValue& Value);
};
