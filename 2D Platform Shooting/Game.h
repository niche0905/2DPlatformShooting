#pragma once
#define SFML_STATIC
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Level.h"

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;

const sf::Color BackgroundColor = sf::Color(135, 206, 235, 255);

constexpr float CameraLagging = 10.0f;
constexpr float CameraOffset = 60.0f;


class Game {
private:
    sf::RenderWindow window;
    sf::Clock clock;

    Level level;

    Player player;

    sf::View view;

public:
    // 윈도우 크기와 타이틀 이름을 설정한다
    Game() : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "2D CLIENT"), level(), player(100.0f, 400.0f, level), view(sf::FloatRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT))
    {


        // 시작시 바로 플레어이 중앙으로 옮길지 말지
        //InitView();
    }

    // 카메라 뷰를 Player어 중심으로
    void InitView();

    // 게임을 실행시키고 게임 로직을 굴린다
    void run(); 

    // 유저 Input을 처리한다
    void handleInput();

    // 게임의 객체들을 업데이트한다
    void update(long long deltaTime);

    // 스크롤링을 위한 view 설정 함수
    void Scrolling(long long deltaTime);

    // 게임의 모든 객체를 그린다
    void draw();
};