#pragma once
#include "Level.h"
#include "Player.h"
#include "Dummy.h"
#include "Item.h"
#include "Image.h"
#include "Scene.h"

class GameScene : public Scene {
    sf::RenderWindow window;
    //sf::Clock clock;
    bool match{ false };

    Level level;

    Player* player1;
    
    Player* player2;

    std::list<Item> items;
    sf::View view;

    std::chrono::system_clock::time_point makeTime;

    Image Background;
    Image UI[2];
    sf::Font font;
    sf::Text texts[6];

    std::list<Bullet> enemy_bullets;

public:
    // 윈도우 크기와 타이틀 이름을 설정한다
    GameScene();

    // 카메라 뷰를 Player어 중심으로
    void InitView();

    // 게임을 실행시키고 게임 로직을 굴린다
    virtual void run();

    // 유저 Input을 처리한다
    virtual void handleInput();

    // 게임의 객체들을 업데이트한다
    virtual void update(long long deltaTime);

    // 총알 충돌 처리 (본인을 제외한 Player)
    void bulletHit();

    // 아이템 충돌 처리
    void eatItem();

    // 아이템 생성
    void makeItem();

    // 아이템 생성 (item create 패킷을 받아서 하는 것)
    void makeItem(uint32_t i_id, float x, float y);

    // 스크롤링을 위한 view 설정 함수
    void Scrolling(long long deltaTime);

    // 게임의 모든 객체를 그린다
    virtual void draw();

    // 씬에 Bullet 추가
    void AddEnemyBullet(float x, float y, bool direction, uint32_t type);

    // 문자 객체 초기화
    void InitText();
    void updateTexts();
    void drawTexts();

    Player& GetPlayer1() { return *player1; }
    Player& GetPlayer2() { return *player2; }
    Player& GetControlPlayer();
    Player& GetOtherPlayer();
};