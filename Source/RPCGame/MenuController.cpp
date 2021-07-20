// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuController.h"

#include "MenuWidget.h"
#include "RPCHelper.h"
#include "RPCInstance.h"
#include "Kismet/GameplayStatics.h"

void AMenuController::BeginPlay()
{
	Super::BeginPlay();

	//设定输入模式
	bShowMouseCursor = true;
	FInputModeUIOnly InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(InputMode);

	//获取GameInstance;
	URPCInstance * RPCInstance=Cast<URPCInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	RPCInstance->AssignPlayerController(this);
	//创建UI
	UClass* MenuWidgetClass =LoadClass<UMenuWidget>(NULL,TEXT("WidgetBlueprint'/Game/Blueprint/MenuWidget_BP.MenuWidget_BP_C'"));
	UMenuWidget* MenuWidget = CreateWidget<UMenuWidget>(GetWorld(),MenuWidgetClass);
	MenuWidget->AddToViewport();

	MenuWidget->AssignRPCInstance(RPCInstance);
}

void AMenuController::EchoNetMode()
{
	ENetMode NetMode = GetNetMode();

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
	default:
		break;
	}
}
