// Copyright Epic Games, Inc. All Rights Reserved.


#include "RPCGameGameModeBase.h"

#include "RPCController.h"
#include "RPCHelper.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "TP_ThirdPerson/TP_ThirdPersonCharacter.h"

ARPCGameGameModeBase::ARPCGameGameModeBase()
{
	PlayerControllerClass=ARPCController::StaticClass();
	DefaultPawnClass = NULL;

	PlayerCount=0;
}

void ARPCGameGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (NewPlayer->GetPawn())
	{
		GetWorld()->DestroyActor(NewPlayer->GetPawn());
	}

	TArray<AActor*>ActorArray;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(),APlayerStart::StaticClass(),ActorArray);

	if (ActorArray.Num()>0)
	{
		PlayerCount++;
		ATP_ThirdPersonCharacter* NewCharacter = GetWorld()->SpawnActor<ATP_ThirdPersonCharacter>(ActorArray[0]->GetActorLocation()+FVector(0.0f,PlayerCount*200.0f,0.0f),ActorArray[0]->GetActorRotation());
		NewPlayer->Possess(NewCharacter);
		DDH::Debug()<<NewPlayer->GetName()<<"Login"<<DDH::End();
	}
}

void ARPCGameGameModeBase::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	PlayerCount--;
	DDH::Debug()<<Exiting->GetName()<<"Logout"<<DDH::End();
}
