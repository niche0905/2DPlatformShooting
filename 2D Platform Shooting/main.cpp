#include "pch.h"
#include "Scene.h"
#include "ClientNetworkManager.h"

Scene game;
ClientNetworkManager network_mgr;

      //-----//
     //     //  (�â���`)��
    //-----//
int main()//
  //-----// Game �����ϱ� ���� Main
{
    network_mgr.Init();
    network_mgr.Connect();
    game.run();

    return 0;
}
