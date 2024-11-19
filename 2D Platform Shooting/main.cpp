#include "pch.h"
#include "SceneManager.h"

      //-----//
     //     //  (っ´ω`)っ
    //-----//
int main()//
  //-----// Game 실행하기 위한 Main
{
    SceneManager* sceneManager = new SceneManager();
    network_mgr.Init(sceneManager);
    sceneManager->GetActiveScene()->run();
    return 0;
}
