// Fill out your copyright notice in the Description page of Project Settings.


#include "RPCActor.h"

#include "CubeNoReplicate.h"
#include "CubeReplicate.h"
#include "RPCHelper.h"

// Sets default values
ARPCActor::ARPCActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
    bNetLoadOnClient = true;
}

// Called when the game starts or when spawned
void ARPCActor::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld()->IsServer())
	{
		GetWorld()->SpawnActor<ACubeReplicate>(ACubeReplicate::StaticClass(),GetActorLocation()+FVector::RightVector*300.0F,FQuat::Identity.Rotator());
		GetWorld()->SpawnActor<ACubeNoReplicate>(ACubeNoReplicate::StaticClass(),GetActorLocation()-FVector::RightVector*300.0F,FQuat::Identity.Rotator());
	}
	//EChoNetMode();
}

void ARPCActor::EChoNetMode()
{
	ENetMode NetMode=GetNetMode();
	switch (NetMode)
	{
	case NM_Standalone:
		DDH::Debug()<<"NM_Standalone"<<DDH::End();
		break;
	case NM_DedicatedServer:
		DDH::Debug()<<"NM_DedicatedServer"<<DDH::End();
		break;
	case NM_ListenServer:
		DDH::Debug()<<"NM_ListenServer"<<DDH::End();
		break;
	case NM_Client:
		DDH::Debug()<<"NM_Client"<<DDH::End();
		break;
	case NM_MAX:
		DDH::Debug()<<"NM_MAX"<<DDH::End();
		break;
	default: ;
	}
}

// Called every frame
void ARPCActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

