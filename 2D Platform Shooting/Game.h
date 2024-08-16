#pragma once
#define SFML_STATIC
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Level.h"


class Game {
private:
    sf::RenderWindow window;
    sf::Clock clock;
    Player player;
    Level level;

public:
    // 윈도우 크기와 타이틀 이름을 설정한다
    Game() : window(sf::VideoMode(800, 600), "2D CLIENT"), level(), player(100.0f, 400.0f, level) { }

    // 게임을 실행시키고 게임 로직을 굴린다
    void run(); 

    // 유저 Input을 처리한다
    void handleInput();

    // 게임의 객체들을 업데이트한다
    void update(long long deltaTime);

    // 게임의 모든 객체를 그린다
    void draw();
};