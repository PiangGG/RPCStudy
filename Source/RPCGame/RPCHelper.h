// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RPCHelper.generated.h"

class RPCGAME_API DDRecord
{
	
private:
	//自身单例
	static TSharedPtr<DDRecord> RecordInst;

	//最终输出字符串
	FString RecordInfo;
	//显示时长
	float ShowTime;
	//显示颜色
	FColor ShowColor;
public:
	//状态模式,0:Debug,1:Log,2:warning,3:Error
	uint8 PatternID;
public:
	inline DDRecord(){};
	~DDRecord(){};
	static TSharedPtr<DDRecord> Get();

	inline void InitParam(float InTime,FColor InColor)
	{
		ShowTime = InTime;
		ShowColor=InColor;
	}

	inline void Output()
	{
		switch (PatternID)
		{
			case 0:
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1,ShowTime,ShowColor,RecordInfo);
				}
				break;
			case 1:
				UE_LOG(LogTemp,Log,TEXT("%S"),*RecordInfo);
				break;
			case 2:
				UE_LOG(LogTemp,Warning,TEXT("%S"),*RecordInfo);
				break;
			case 3:
				UE_LOG(LogTemp,Error,TEXT("%S"),*RecordInfo);
				break;
			default:
				break;
		}
		
		RecordInfo.Empty();
	}

	//移位操作符重写
	inline DDRecord&operator<<(FString Info){RecordInfo.Append(Info);return *this;}
	inline DDRecord&operator<<(FName Info){RecordInfo.Append(Info.ToString());return *this;}
	inline DDRecord&operator<<(FText Info){RecordInfo.Append(Info.ToString());return *this;}
	inline DDRecord&operator<<(const char* Info){RecordInfo+=Info;return *this;}
	inline DDRecord&operator<<(const char Info){RecordInfo.AppendChar(Info);return *this;}
	inline DDRecord&operator<<(int32 Info){RecordInfo.Append(FString::FromInt(Info));return *this;}
	inline DDRecord&operator<<(float Info){RecordInfo.Append(FString::SanitizeFloat(Info));return *this;}
	inline DDRecord&operator<<(double Info){RecordInfo.Append(FString::SanitizeFloat(Info));return *this;}
	inline DDRecord&operator<<(bool Info){RecordInfo.Append(Info?FString("true"):FString("false"));return *this;}
	inline DDRecord&operator<<(FVector2D Info){RecordInfo.Append(Info.ToString());return *this;}
	inline DDRecord&operator<<(FVector Info){RecordInfo.Append(Info.ToString());return *this;}
	inline DDRecord&operator<<(FRotator Info){RecordInfo.Append(Info.ToString());return *this;}
	inline DDRecord&operator<<(FQuat Info){RecordInfo.Append(Info.ToString());return *this;}
	inline DDRecord&operator<<(FTransform Info){RecordInfo.Append(Info.ToString());return *this;}
	inline DDRecord&operator<<(FMatrix Info){RecordInfo.Append(Info.ToString());return *this;}
	inline DDRecord&operator<<(FColor Info){RecordInfo.Append(Info.ToString());return *this;}
	inline DDRecord&operator<<(FLinearColor Info){RecordInfo.Append(Info.ToString());return *this;}

	//输出
	inline void operator<<(DDRecord& Record){Record.Output();}
};

namespace DDH
{
	FORCEINLINE DDRecord& Debug(float InTime = 3000.0f,FColor InColor = FColor::Yellow)
	{
		DDRecord::Get()->PatternID = 0;
		DDRecord::Get()->InitParam(InTime,InColor);
		return *DDRecord::Get();
	}
	FORCEINLINE DDRecord& Debug(FColor InColor)
	{
		return Debug(3000.0,InColor);
	}
	FORCEINLINE DDRecord&Log()
	{
		DDRecord::Get()->PatternID = 1;
		return *DDRecord::Get();
	}
	FORCEINLINE DDRecord&Warning()
	{
		DDRecord::Get()->PatternID = 2;
		return *DDRecord::Get();
	}
	FORCEINLINE DDRecord&Error()
	{
		DDRecord::Get()->PatternID = 3;
		return *DDRecord::Get();
	}
	FORCEINLINE DDRecord&End()
	{
		return *DDRecord::Get();
	}
	
}
/**
 * 
 */
UCLASS()
class RPCGAME_API URPCHelper : public UObject
{
	GENERATED_BODY()
	
};
