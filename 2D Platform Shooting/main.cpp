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
    guns.reserve(9);
    guns.push_back(Gun("Pistol", 10.0f, 500.0f, -1, 450, -1));  // 기본 총으로 탄창이 무한하게
    guns.push_back(Gun("AK-47", 20.0f, 600.0f, 60, 600, 1));
    guns.push_back(Gun("M4A1", 15.0f, 640.0f, 75, 700, 1));
    guns.push_back(Gun("MP5", 12.0f, 560.0f, 90, 900, 1));
    guns.push_back(Gun("P90", 10.0f, 700.0f, 120, 1000, 1));
    guns.push_back(Gun("Uzi", 15.0f, 800.0f, 90, 600, 1));
    guns.push_back(Gun("M249", 17.0f, 750.0f, 200, 800, 1));
    guns.push_back(Gun("Barrett-M82", 100.0f, 600.0f, 3, 60, 1));
    guns.push_back(Gun("AWM", 80.0f, 800.0f, 5, 120, 1));

    saveGunFromFile(GunSavePath, guns);


    Game game;
    game.run();

    return 0;
}
