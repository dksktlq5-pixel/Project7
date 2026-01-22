// Copyright Epic Games, Inc. All Rights Reserved.

#include "Project_7GameMode.h"
#include "Project_7Character.h"
#include "UObject/ConstructorHelpers.h"

AProject_7GameMode::AProject_7GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
