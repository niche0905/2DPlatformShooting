#include "pch.h"
#include "Scene.h"
#include "SceneManager.h"
#include "ClientNetworkManager.h"

SceneManager scene_manager;
ClientNetworkManager network_mgr;

      //-----//
     //     //  (�â���`)��
    //-----//
int main()//
  //-----// Game �����ϱ� ���� Main
{
    network_mgr.Init();
    network_mgr.Connect();
    scene_manager.GetActiveScene()->run();

    return 0;
}
