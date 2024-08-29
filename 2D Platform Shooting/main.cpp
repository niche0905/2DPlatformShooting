#include "Game.h"
#include "Gun.h"
#include <iostream>
#include <fstream>

      //-----//
     //     //  (っ´ω`)っ
    //-----//
int main()//
  //-----// Game 실행하기 위한 Main
{
    std::ifstream inFile(GunSavePath);
    Gun gun1, gun2;
    inFile >> gun1 >> gun2;
    inFile.close();

    std::cout << gun1 << std::endl;
    std::cout << gun2 << std::endl;

    //Game game;
    //game.run();

    return 0;
}
