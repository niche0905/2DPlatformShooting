#include "pch.h"
#include "SceneManager.h"
#include "ClientNetworkManager.h"

SceneManager sceneManager;
ClientNetworkManager network_mgr;

      //-----//
     //     //  (�â���`)��
    //-----//
int main()//
  //-----// Game �����ϱ� ���� Main
{
    network_mgr.Init();
    network_mgr.Connect();
    sceneManager.GetActiveScene()->run();
    return 0;
}
