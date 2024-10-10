#pragma once
#include "Level.h"
#include "Player.h"
#include "Item.h"

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;

const sf::Color BackgroundColor = sf::Color(135, 206, 235, 255);

constexpr float CameraLagging = 10.0f;
constexpr float CameraOffset = 60.0f;

const std::string GunSavePath = "./Saved/Guns/GunAttribute.txt";

class Game {
private:
    sf::RenderWindow window;
    sf::Clock clock;

    Level level;

    std::vector<Player> players;

    std::list<Item> items;

    //Dummy dummy;

    // 민경원 코멘트
    // Game 클래스 안에 txt로 총들 받아서 guns에 넣었는데
    // 어차피 총 먹고 바꾸고 하는건 player가 하는 일인데
    // 그러면 굳이 guns를 Game에 넣지 말고 Player 클래스에 넣는 게 더 좋아보이는데
    // 어떻게 할지 모루것당

    // [cham] 9.22: 
    // gun에 대한 정보는 unordered_map으로 전역으로 저장해서 사용하자.
    // player는 gun의 id값만을 가지도록 한다.
    // std::vector<Gun> guns;

    // 송승호 코멘트
    // Player가 단일 객체로 있는게 아니라 자료구조에 있어야함
    // 그렇게 플레이어들이 있어서 플레이어들 끼리 총알 충돌 검사를 해야함
    // ㄴ 한 PC에서 두명이서 하는게 목표이면

    sf::View view;

    std::chrono::system_clock::time_point makeTime;

public:
    // 윈도우 크기와 타이틀 이름을 설정한다
    Game() : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "2D CLIENT"), level(), view(sf::FloatRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT))
    {
        makeTime = std::chrono::system_clock::now();
        players.emplace_back(100.0f, 400.0f, &level);
        players.emplace_back(500.0f, 400.0f, &level, sf::Keyboard::I, sf::Keyboard::K, sf::Keyboard::J, sf::Keyboard::L, sf::Keyboard::Enter);

        items.emplace_back(200.0f, -500.0f, &level);
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

    // 총알 충돌 처리 (본인을 제외한 Player)
    void bulletHit();

    // 아이템 충돌 처리
    void eatItem();

    // 아이템 생성
    void makeItem();

    // 스크롤링을 위한 view 설정 함수
    void Scrolling(long long deltaTime);

    // 게임의 모든 객체를 그린다
    void draw();

    void buildGun();

    bool loadGunFromFile(const std::string& filePath);
    void saveGunFromFile(const std::string& filePath);
};