#include "pch.h"
#include "SceneManager.h"

// �� �̱����� �ȵ���
//SceneManager sceneManager = GET_SINGLE(SceneManager);
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
