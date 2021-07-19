// Fill out your copyright notice in the Description page of Project Settings.


#include "FireEffectActor.h"

#include "Components/TextRenderComponent.h"
#include "Net/UnrealNetwork.h"
#include "Particles/ParticleSystemComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AFireEffectActor::AFireEffectActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent=CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	
	FireEffect=CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FireEffact"));
	FireEffect->SetupAttachment(RootComponent);
	
	TextRender=CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRender"));
	TextRender->SetupAttachment(RootComponent);
	
	SetReplicates(true);
}

// Called when the game starts or when spawned
void AFireEffectActor::BeginPlay()
{
	Super::BeginPlay();

	CountDownTimer = 20.0f;

	if (GetWorld()->IsServer())
	{
		FTimerDelegate UpdateTimerDelegate=FTimerDelegate::CreateUObject(this,&AFireEffectActor::UpdateTimer);
		GetWorld()->GetTimerManager().SetTimer(UpdateTimerHandle,UpdateTimerDelegate,1.0f,true);
	}
}

// Called every frame
void AFireEffectActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//渲染数字
	TextRender->SetText(FText::FromString(FString::FromInt(CountDownTimer)));
}

void AFireEffectActor::UpdateTimer()
{
	if (CountDownTimer>0)
	{
		CountDownTimer-=1;
	}else
	{
		Deactivate =!Deactivate;
		OnRep_Deactivate();
	}
}

void AFireEffectActor::OnRep_Deactivate()
{
	FireEffect->Deactivate();
}

void AFireEffectActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AFireEffectActor,CountDownTimer);
	DOREPLIFETIME(AFireEffectActor,Deactivate);
	
}
