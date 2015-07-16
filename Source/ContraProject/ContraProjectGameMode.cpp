// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "ContraProject.h"
#include "ContraProjectGameMode.h"
#include "ContraProjectCharacter.h"

AContraProjectGameMode::AContraProjectGameMode()
{
	// set default pawn class to our character
	DefaultPawnClass = AContraProjectCharacter::StaticClass();	
}
