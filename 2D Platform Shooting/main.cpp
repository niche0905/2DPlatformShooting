#include "pch.h"
#include "Game.h"
#include "ClientNetworkManager.h"

Game game;
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
