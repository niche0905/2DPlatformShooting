#include "pch.h"
#include "SceneManager.h"

      //-----//
     //     //  (�â���`)��
    //-----//
int main()//
  //-----// Game �����ϱ� ���� Main
{
    SceneManager* sceneManager = new SceneManager();
    network_mgr.Init(sceneManager);
    sceneManager->GetActiveScene()->run();
    return 0;
}
