#include "Game.h"
#include "Gun.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

      //-----//
     //     //  (�â���`)��
    //-----//
int main()//
  //-----// Game �����ϱ� ���� Main
{
    std::vector<Gun> guns;
    loadGunFromFile(GunSavePath, guns);

    for (const Gun& gun : guns)
        std::cout << gun << std::endl;


    Game game;
    game.run();

    return 0;
}
