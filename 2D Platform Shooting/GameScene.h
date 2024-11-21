#pragma once
#include "Level.h"
#include "Player.h"
#include "Dummy.h"
#include "Item.h"
#include "Image.h"
#include "Scene.h"

class GameScene : public Scene {
    sf::RenderWindow window;
    sf::Clock clock;

    Level level;

    Player* player;
    
    Dummy* dummy_enemy;

    std::list<Item> items;
    sf::View view;

    std::chrono::system_clock::time_point makeTime;

    Image Background;
    Image UI[2];
    sf::Font font;
    sf::Text texts[6];

public:
    // ������ ũ��� Ÿ��Ʋ �̸��� �����Ѵ�
    GameScene(uint32_t p_id);

    // ī�޶� �並 Player�� �߽�����
    void InitView();

    // ������ �����Ű�� ���� ������ ������
    virtual void run();

    // ���� Input�� ó���Ѵ�
    virtual void handleInput();

    // ������ ��ü���� ������Ʈ�Ѵ�
    virtual void update(long long deltaTime);

    // �Ѿ� �浹 ó�� (������ ������ Player)
    void bulletHit();

    // ������ �浹 ó��
    void eatItem();

    // ������ ����
    void makeItem();

    // ��ũ�Ѹ��� ���� view ���� �Լ�
    void Scrolling(long long deltaTime);

    // ������ ��� ��ü�� �׸���
    virtual void draw();

    // ���� ��ü �ʱ�ȭ
    void InitText();
    void updateTexts();
    void drawTexts();

    Player& GetPlayers() { return *player; }
    Dummy& GetDummyEnemy() { return *dummy_enemy; }
};