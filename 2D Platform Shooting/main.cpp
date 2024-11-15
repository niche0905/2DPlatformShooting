#include "pch.h"
#include "SceneManager.h"
#include "ClientNetworkManager.h"

SceneManager sceneManager;
ClientNetworkManager network_mgr;

      //-----//
     //     //  (っ´ω`)っ
    //-----//
int main()//
  //-----// Game 실행하기 위한 Main
{
    network_mgr.Init();
    network_mgr.Connect();
    sceneManager.GetActiveScene()->run();
    return 0;
}
