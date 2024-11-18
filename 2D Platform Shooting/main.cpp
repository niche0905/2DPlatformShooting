#include "pch.h"
#include "SceneManager.h"

// 왜 싱글톤이 안되지
//SceneManager sceneManager = GET_SINGLE(SceneManager);
SceneManager sceneManager;

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
