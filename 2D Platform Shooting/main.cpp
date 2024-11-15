#include "pch.h"
#include "Scene.h"
#include "SceneManager.h"
#include "ClientNetworkManager.h"

SceneManager scene_manager;
ClientNetworkManager network_mgr;

      //-----//
     //     //  (っ´ω`)っ
    //-----//
int main()//
  //-----// Game 실행하기 위한 Main
{
    network_mgr.Init();
    network_mgr.Connect();
    scene_manager.GetActiveScene()->run();

    return 0;
}
