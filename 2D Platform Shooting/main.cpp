#include "Game.h"
#include "Gun.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

      //-----//
     //     //  (っ´ω`)っ
    //-----//
int main()//
  //-----// Game 실행하기 위한 Main
{
    std::vector<Gun> guns;
    loadGunFromFile(GunSavePath, guns);

    for (const Gun& gun : guns)
        std::cout << gun << std::endl;


    Game game;
    game.run();

    return 0;
}
