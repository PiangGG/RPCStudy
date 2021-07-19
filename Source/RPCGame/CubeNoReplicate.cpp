// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeNoReplicate.h"

#include "RPCHelper.h"

// Sets default values
ACubeNoReplicate::ACubeNoReplicate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent=CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	CubeMesh=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMesh"));
	CubeMesh->SetupAttachment(RootComponent);

	ConstructorHelpers::FObjectFinder<UStaticMesh>StaticMesh(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_WideCapsule.Shape_WideCapsule'"));
	CubeMesh->SetStaticMesh(StaticMesh.Object);
	SetReplicates(false);
}

// Called when the game starts or when spawned
void ACubeNoReplicate::BeginPlay()
{
	Super::BeginPlay();
	//DDH::Debug()<<"IsServer-->"<<GetWorld()->IsServer()<<";HasAuthority()-->"<<HasAuthority()<<"ACubeNoReplicate BeginPlay"<<DDH::End();
}

// Called every frame
void ACubeNoReplicate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

