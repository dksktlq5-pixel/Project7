// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PawnController.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class PROJECT_7_API APawnController : public APlayerController
{
	GENERATED_BODY()
	
public:
	APawnController();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")	
	UInputMappingContext* InputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* SprintAction;

	virtual void BeginPlay() override;
};
