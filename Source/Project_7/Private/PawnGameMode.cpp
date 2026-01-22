// PawnGameMode.cpp
#include "PawnGameMode.h"
#include "MyPawn.h"
#include "PawnController.h"

APawnGameMode::APawnGameMode()
{
    DefaultPawnClass = AMyPawn::StaticClass();
    PlayerControllerClass = APawnController::StaticClass();
}
