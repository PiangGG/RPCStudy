// Fill out your copyright notice in the Description page of Project Settings.


#include "RPCController.h"

void ARPCController::BeginPlay()
{
	bShowMouseCursor = false;

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
}
