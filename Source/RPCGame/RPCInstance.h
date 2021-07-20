// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "Interfaces/OnlineSessionInterface.h"
#include "RPCInstance.generated.h"

class IOnlineSubsystem;
class APlayerController;
/**
 * 
 */

UCLASS()
class RPCGAME_API URPCInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	URPCInstance();
	
	void AssignPlayerController(APlayerController*InController);
	
	void HostSession();

	void ClientSession();

	void DestroySession();
protected:
	void OnCreateSessionComplete(FName SessionName,bool bWasSuccessful);
	void OnStartOnlineGameComplete(FName SessionName,bool bWasSuccessful);
	void OnFindSessionsComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName SessionName,EOnJoinSessionCompleteResult::Type Result);
	void OnDestroySessionComplete(FName SessionName,bool bWasSuccessful);
protected:
	APlayerController* PlayerController;
	
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;//委托
	FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;
	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;
	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;
	FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;
	
	FDelegateHandle OnCreateSessionCompleteDelegateHandle;//句柄
	FDelegateHandle OnStartSessionCompleteDelegateHandle;
	FDelegateHandle OnFindSessionCompleteDelegateHandle;
	FDelegateHandle OnJoinSessionCompleteDelegateHandle;
	FDelegateHandle OnDestrySessionCompleteDelegateHandle;
	
	IOnlineSubsystem* OnlineSubsystem;
	TSharedPtr<const FUniqueNetId> UserID;
	//保存寻找到的Sessions
	TSharedPtr<FOnlineSessionSearch> SearchObject;
};
