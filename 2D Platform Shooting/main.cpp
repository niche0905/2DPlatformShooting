#include "pch.h"
#include "SceneManager.h"

SceneManager sceneManager;


      //-----//
     //     //  (�â���`)��
    //-----//
int main()//
  //-----// Game �����ϱ� ���� Main
{
    network_mgr.Init();
    sceneManager.GetActiveScene()->run();
    return 0;
}
