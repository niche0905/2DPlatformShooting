#pragma once
#include "Level.h"
#include "Player.h"
#include "Item.h"
#include "Image.h"


// #include "TextureManager.h"

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;

const sf::Color BackgroundColor = sf::Color(135, 206, 235, 255);

constexpr float CameraLagging = 10.0f;
constexpr float CameraOffset = 60.0f;

const std::string GunSavePath = "./Saved/Guns/GunAttribute.txt";

class Scene {
private:
    sf::RenderWindow window;
    sf::Clock clock;

    Level level;

    std::vector<Player> players;

    std::list<Item> items;

    sf::View view;

    std::chrono::system_clock::time_point makeTime;

    Image Background;
    Image UI[2];
    sf::Font font;
    sf::Text texts[6];

public:
    // 윈도우 크기와 타이틀 이름을 설정한다
    Scene();

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

    // 문자 객체 초기화
    void InitText();
    void updateTexts();
    void drawTexts();


    // cham TODO: gun manager에서 작업하도록 변경
    void buildGun();
    bool loadGunFromFile(const std::string& filePath);
    void saveGunFromFile(const std::string& filePath);
};