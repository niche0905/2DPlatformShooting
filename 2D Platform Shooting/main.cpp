#include "Game.h"
#include "Gun.h"
#include <fstream>

      //-----//
     //     //  (�â���`)��
    //-----//
int main()//
  //-----// Game �����ϱ� ���� Main
{
    Gun gun1("test", 0.5, 0.5, 10, 50, 1);
    Gun gun2("myGun", 1.1, 2.5, 20, 70, 2);
    std::ofstream outFile(GunSavePath);
    outFile << gun1 << std::endl;
    outFile << gun2 << std::endl;
    outFile.close();

    return 0;
}
