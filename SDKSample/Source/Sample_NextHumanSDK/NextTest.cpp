#include "NextTest.h"
#include "NextAvatarActor.h"
#include "INextHumanSDK.h"
#include "TestUtils.h"

using namespace nexthuman::sdk::demo;

static TAutoConsoleVariable<FString> CVarNextHumanSDKDemo(
	TEXT("nexthuman.sdk.demo"),
	TEXT(""),
	TEXT("Run NextHumanSDK Demo\n"),
	ECVF_Default
);

static void CVarNextHumanSDKDemoSinkFunction() {
	FString InputCommand = CVarNextHumanSDKDemo.GetValueOnGameThread();
	if (InputCommand.IsEmpty()) {
		return;
	}

#define NEXTHUMAN_SDK_DEMO_GET_ARG_1(Command, Arg, Match)\
FString Arg;\
FParse::Value(*Command, Match, Arg);\
if (Arg.IsEmpty()) {\
	UE_LOG(LogTemp, Warning, TEXT("no %s specified, %s"), ANSI_TO_TCHAR(#Arg), *InputCommand);\
	return;\
}

#define NEXTHUMAN_SDK_DEMO_GET_ARG_INT_1(Command, Arg, Match)\
int32 Arg;\
FParse::Value(*Command, Match, Arg);

#define NEXTHUMAN_SDK_DEMO_GET_ARG_2(Command, Arg, Match, ShortMatch)\
FString Arg;\
FParse::Value(*Command, Match, Arg);\
if (Arg.IsEmpty()) {\
	FParse::Value(*Command, ShortMatch, Arg);\
}\
if (Arg.IsEmpty()) {\
	UE_LOG(LogTemp, Warning, TEXT("no %s specified, %s"), ANSI_TO_TCHAR(#Arg), *InputCommand);\
	return;\
}

	NEXTHUMAN_SDK_DEMO_GET_ARG_2(InputCommand, Target, TEXT("-target="), TEXT("-t="));

	if (Target == TEXT("Avatar")) {
		NEXTHUMAN_SDK_DEMO_GET_ARG_2(InputCommand, Action, TEXT("-action="), TEXT("-a="));
		if (Action == TEXT("Create")) {
			NEXTHUMAN_SDK_DEMO_GET_ARG_1(InputCommand, Id, TEXT("-id="));
			FTestUtils::CreateAvatar(Id);
		}
		else if (Action == TEXT("List")) {

		}
		else if (Action == TEXT("AddBundle")) {
			NEXTHUMAN_SDK_DEMO_GET_ARG_1(InputCommand, Id, TEXT("-id="));
			NEXTHUMAN_SDK_DEMO_GET_ARG_INT_1(InputCommand, Index, TEXT("-index="));
			FTestUtils::AddBundle(Index, Id);
		}
	}
}

static FAutoConsoleVariableSink CVarNextHumanSDKDemoSink(FConsoleCommandDelegate::CreateStatic(&CVarNextHumanSDKDemoSinkFunction));