// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RPCController.generated.h"

/**
 * 
 */
UCLASS()
class RPCGAME_API ARPCController : public APlayerController
{
	GENERATED_BODY()

	protected:
	virtual void BeginPlay() override;
};
