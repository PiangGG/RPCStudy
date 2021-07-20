// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RPCGameGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class RPCGAME_API ARPCGameGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	public:
	ARPCGameGameModeBase();
	
	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Logout(AController* Exiting) override;

protected:
	int32 PlayerCount;
};
