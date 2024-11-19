#include "pch.h"
#include "SceneManager.h"

      //-----//
     //     //  (っ´ω`)っ
    //-----//
int main()//
  //-----// Game 실행하기 위한 Main
{
    network_mgr.Init();
    sceneManager.GetActiveScene()->run();
    return 0;
}
