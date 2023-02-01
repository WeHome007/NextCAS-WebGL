环境要求：
1. UE 5.0.3
2. Visual Studio 2022
3. Windows SDK 10.0.22621.0
4. 下载SDK：链接：https://pan.baidu.com/s/1covDRq_sXC90rNbHmdgOyw?pwd=18p4
5. 解压到UE的安装目录下：UE/Engine/Plugins/Marketplace。
   解压后的路径应该为：D:\UE_5.0\Engine\Plugins\Marketplace\NextHuman\Release\...

打包：
1. 右键点击Sample_NextHumanSDK.uproject，选择“Generate Visual Studio project files”。成功后，可以看到“Sample_NextHumanSDK.sln”
2. 双击“Sample_NextHumanSDK.sln”
3. 修改TestUtils.cpp中的FTestUtils::AccessToken为自己的AccessToken
4. 选择Visual Studio菜单，Build/Build Solution
5. 构建成功后，选择菜单Debug/Start Without Debugging，可以看到UE编辑器启动
6. 选择 Platforms/Windows/Package Project，进行打包
