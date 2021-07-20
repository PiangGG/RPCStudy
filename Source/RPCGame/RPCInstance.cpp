// Fill out your copyright notice in the Description page of Project Settings.


#include "RPCInstance.h"
#include  "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSubsystemUtils.h"
#include "RPCHelper.h"
#include "Kismet/GameplayStatics.h"

URPCInstance::URPCInstance()
{
	//绑定回调函数
	OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this,&URPCInstance::OnCreateSessionComplete);

	OnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this,&URPCInstance::OnStartOnlineGameComplete);

	OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this,&URPCInstance::OnFindSessionsComplete);

	OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this,&URPCInstance::OnJoinSessionComplete);

	OnDestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this,&URPCInstance::OnDestroySessionComplete);
}

void URPCInstance::AssignPlayerController(APlayerController* InController)
{
	PlayerController = InController;

	//获取Online
	//第一种获取方式:Online::GetSubsystem
	//第二种获取方式:使用IOnlineSubsystem
	OnlineSubsystem = Online::GetSubsystem(PlayerController->GetWorld(),NAME_None);

	//获取UserID
	//获取方式1
	if (GetLocalPlayers().Num()==0)
	{
		DDH::Debug()<<"没有本地玩家存在"<<DDH::End();
	}else
	{
		UserID = (*GetLocalPlayers()[0]->GetPreferredUniqueNetId()).AsShared();
	}
	
#if 0
	//获取方式2|打包运行exe无问题,但在编辑器下运行多个窗口找不到PlayerState
	if(PlayerController->PlayerState)
	{
		UserID=PlayerController->PlayerState->UniqueId.GetUniqueNetId();
	}
#endif
	//在这里直接获取Session运行时会报错,生命周期的问题
}

void URPCInstance::HostSession()
{
	if (OnlineSubsystem)
	{
		IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface();

		if (Session.IsValid())
		{
			//绘画设置
			FOnlineSessionSettings Settings;
			//连接数
			Settings.NumPublicConnections=10;
			Settings.bShouldAdvertise = true;
			Settings.bAllowJoinInProgress = true;
			//使用局域网
			Settings.bIsLANMatch = true;
			Settings.bAllowJoinViaPresence = true;
			//绑定委托
			OnCreateSessionCompleteDelegateHandle = Session->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);
			Session->CreateSession(*UserID,NAME_GameSession,Settings);
		}
	}
}

void URPCInstance::ClientSession()
{
	if (OnlineSubsystem)
	{
		IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface();

		if (Session.IsValid())
		{
			//实例化搜索结果并且设定参数
			SearchObject = MakeShareable(new FOnlineSessionSearch);
			SearchObject->MaxSearchResults = 10;
			SearchObject->bIsLanQuery = true;
			SearchObject->QuerySettings.Set(SEARCH_PRESENCE,true,EOnlineComparisonOp::Equals);
			OnFindSessionCompleteDelegateHandle = Session->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);
			//进行会话寻找
			Session->FindSessions(*UserID,SearchObject.ToSharedRef());
		}
	}
}

void URPCInstance::DestroySession()
{
	if (OnlineSubsystem)
	{
		IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface();

		if (Session.IsValid())
		{
			//销毁会话委托
			OnDestrySessionCompleteDelegateHandle = Session->AddOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegate);
			//执行销毁会话
			Session->DestroySession(NAME_GameSession);
		}
	}
}

void URPCInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (OnlineSubsystem)
	{
		IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface();

		if (Session.IsValid())
		{
			//解绑创建会话完成回调函数
			Session->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);

			//判断床架是否成功
			if (bWasSuccessful)
			{
				DDH::Debug()<<"CreateSession Succeed"<<DDH::End();
				//绑定开启委托会话
				OnStartSessionCompleteDelegateHandle = Session->AddOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegate);
				//执行开启会话
				Session->StartSession(NAME_GameSession);
			}else
			{
				DDH::Debug()<<"CreateSession Failed"<<DDH::End();
			}
		}
	}
}

void URPCInstance::OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful)
{
	if (OnlineSubsystem)
	{
		IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface();

		if (Session.IsValid())
		{
			//注销开启会话委托绑定
			Session->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);
			
			if (bWasSuccessful)
			{
				DDH::Debug()<<"StartSession Succeed"<<DDH::End();
				//绑定开启委托会话
				OnStartSessionCompleteDelegateHandle = Session->AddOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegate);
				//服务端跳转场景
				UGameplayStatics::OpenLevel(PlayerController->GetWorld(),FName("GameMap"),true,FString("listen"));
			}else
			{
				DDH::Debug()<<"StartSession Failed"<<DDH::End();
			}
		}
	}
}

void URPCInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
	if (OnlineSubsystem)
	{
		IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface();

		if (Session.IsValid())
		{
			Session->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);
			if (bWasSuccessful)
			{
				//如果搜索的结果存在并且大于1
				if (SearchObject.IsValid()&&SearchObject->SearchResults.Num()>0)
				{
					DDH::Debug()<<"FindSession Succeed"<<DDH::End();
					OnJoinSessionCompleteDelegateHandle = Session->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);
					Session->JoinSession(*UserID,NAME_GameSession,SearchObject->SearchResults[0]);
				}else
				{
					DDH::Debug()<<"FindSession Succeed"<<DDH::End();
				}
			}
		}
	}
}

void URPCInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (OnlineSubsystem)
	{
		IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface();

		if (Session.IsValid())
		{
			Session->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);

			if (Result == EOnJoinSessionCompleteResult::Success)
			{
				//传送玩家到新地图
				FString ConnectString;
				if (Session->GetResolvedConnectString(NAME_GameSession,ConnectString))
				{
					DDH::Debug()<<"JoinSession Succeed"<<DDH::End();
					//客户端切换到服务器关卡
					PlayerController->ClientTravel(ConnectString,TRAVEL_Absolute);
				}
			}else
			{
				DDH::Debug()<<"JoinSession Failed"<<DDH::End();
			}
		}
	}
}

void URPCInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (OnlineSubsystem)
	{
		IOnlineSessionPtr Session = OnlineSubsystem->GetSessionInterface();

		if (Session.IsValid())
		{
			Session->ClearOnDestroySessionCompleteDelegate_Handle(OnDestrySessionCompleteDelegateHandle);
			//其他逻辑
		}
	}
}
