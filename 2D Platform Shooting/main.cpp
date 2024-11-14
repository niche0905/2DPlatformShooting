#include "pch.h"
#include "Scene.h"
#include "ClientNetworkManager.h"

Scene game;
ClientNetworkManager network_mgr;

      //-----//
     //     //  (っ´ω`)っ
    //-----//
int main()//
  //-----// Game 실행하기 위한 Main
{
    network_mgr.Init();
    network_mgr.Connect();
    game.run();

    return 0;
}
