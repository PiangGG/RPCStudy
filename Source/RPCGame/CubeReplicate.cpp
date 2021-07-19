// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeReplicate.h"

#include "RPCHelper.h"

// Sets default values
ACubeReplicate::ACubeReplicate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent=CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	CubeMesh=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMesh"));
	CubeMesh->SetupAttachment(RootComponent);

	ConstructorHelpers::FObjectFinder<UStaticMesh>StaticMesh(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cylinder.Shape_Cylinder'"));
	CubeMesh->SetStaticMesh(StaticMesh.Object);
	SetReplicates(true);
}

// Called when the game starts or when spawned
void ACubeReplicate::BeginPlay()
{
	Super::BeginPlay();
	//DDH::Debug()<<"IsServer-->"<<GetWorld()->IsServer()<<";HasAuthority()-->"<<HasAuthority()<<"ACubeReplicate BeginPlay"<<DDH::End();
}

// Called every frame
void ACubeReplicate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

