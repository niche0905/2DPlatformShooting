#include "pch.h"
#include "SceneManager.h"
#include "ClientNetworkManager.h"

SceneManager acvite_scene;
ClientNetworkManager network_mgr;

      //-----//
     //     //  (�â���`)��
    //-----//
int main()//
  //-----// Game �����ϱ� ���� Main
{
    network_mgr.Init();
    network_mgr.Connect();
    acvite_scene.GetActiveScene()->run();

    return 0;
}
