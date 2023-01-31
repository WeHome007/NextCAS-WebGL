#include "TestUtils.h"
#include "NextAvatarActor.h"

#include "Kismet/GameplayStatics.h"

using namespace nexthuman::sdk::demo;

const FString FTestUtils::AccessToken = TEXT("TZNFxFGY7hxsFRyZksT01iDi+K5b1F8+EBSwatLy0pJdmoZGaXVBiM6yEymgtAf2ixZlUE12+thK2ndKcDcyKYvLMxql0UPti4guFoE3r2CE7+ghnxSOZIFIj6+fxyVATO5TsXQVNK+3UDxsCAVrOjJpANIIEQ14Z1bAdBSj11s=");

void FTestUtils::Run(TFunction<void()> Runnable) {
	if (INextHumanSDKModule::Get().IsInitialized()) {
		Runnable();
	}
	else {
		INextHumanSDKModule::Get().Initialize(AccessToken, [=](bool Initialized)
		{
			if (Initialized) {
				Runnable();
			}
		});
	}
}

void FTestUtils::CreateAvatar(const FString& AvatarId) {
	if (AvatarId.IsEmpty()) {
		return;
	}

	auto Method = [=]()
	{
		UE_LOG(LogTemp, Warning, TEXT("create avatar"));
		UWorld* World = GEngine->GetWorldContextFromGameViewport(GEngine->GameViewport)->World();
		FVector Location(0, 0, 0);
		FRotator Rotation(0, 0, 0);
		auto Avatar = Cast<ANextAvatarActor>(World->SpawnActor(ANextAvatarActor::StaticClass(), &Location, &Rotation));
		ANextAvatarActor::FNextAvatarCallback Callback;
		Callback.BindLambda([=](bool Success, uint32 Code, const FString& Message)
		{
			UE_LOG(LogTemp, Warning, TEXT(",%d,%d,%s"), Success, Code, *Message);
		});
		Avatar->AddBundle(AvatarId, Callback);
		if (Avatar) {
			UE_LOG(LogTemp, Warning, TEXT("Spawn Success"));
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Spawn failed"));
		}
	};

	Run(Method);
}

void FTestUtils::AddBundle(int32 AvatarIndex, const FString& Id) {
	if (AvatarIndex < 0 || Id.IsEmpty()) {
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("AddBundle"));
	UWorld* World = GEngine->GetWorldContextFromGameViewport(GEngine->GameViewport)->World();
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(World, ANextAvatarActor::StaticClass(), Actors);
	if (AvatarIndex >= Actors.Num()) {
		return;
	}

	for (auto it = Actors.CreateConstIterator(); it; ++it) {
		if (it.GetIndex() == AvatarIndex) {
			ANextAvatarActor* Avatar = Cast<ANextAvatarActor>(*it);
			if (Avatar) {
				ANextAvatarActor::FNextAvatarCallback Callback;
				Callback.BindLambda([=](bool Success, int32 Code, const FString& Message)
				{
					UE_LOG(LogTemp, Warning, TEXT("Avatar(%d) Add Bundle(%s) %d, %d, %s"), AvatarIndex, *Id, Success, Code, *Message);
				});
				Avatar->AddBundle(Id, Callback);
			}
		}
	}
}