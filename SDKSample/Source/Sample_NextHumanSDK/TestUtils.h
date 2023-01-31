#pragma once

namespace nexthuman::sdk::demo {
	class FTestUtils {
	public:
		static const FString AccessToken;
		static void CreateAvatar(const FString& AvatarId);
		static void Run(TFunction<void()> Runnable);
		static void AddBundle(int32 AvatarIndex, const FString& Id);
	};
}