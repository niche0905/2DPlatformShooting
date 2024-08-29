#include "Game.h"
#include "Gun.h"
#include <fstream>

      //-----//
     //     //  (っ´ω`)っ
    //-----//
int main()//
  //-----// Game 실행하기 위한 Main
{
    Gun gun1("test", 0.5, 0.5, 10, 50, 1);
    Gun gun2("myGun", 1.1, 2.5, 20, 70, 2);
    std::ofstream outFile(GunSavePath);
    outFile << gun1 << std::endl;
    outFile << gun2 << std::endl;
    outFile.close();

    return 0;
}
