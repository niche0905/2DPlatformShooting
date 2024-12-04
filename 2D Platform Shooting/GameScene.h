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
    Image titleImage;
    Image resultImage[2];
    sf::Font font;
    sf::Text texts[6];

    std::list<Bullet> enemy_bullets;

public:
    // ������ ũ��� Ÿ��Ʋ �̸��� �����Ѵ�
    GameScene();

    // ī�޶� �並 Player�� �߽�����
    void InitView();

    // ������ �����Ű�� ���� ������ ������
    virtual void run();

    // ���� Input�� ó���Ѵ�
    virtual void handleInput();

    // ������ ��ü���� ������Ʈ�Ѵ�
    virtual void update(long long deltaTime);

    // �Ѿ� �浹 ó�� (������ ������ Player)
    //void bulletHit();

    // ������ �浹 ó��
    //void eatItem();

    // ���� �Ѿ� ������Ʈ
    void updateEnemyBullets(long long deltaTime);

    // ������ ����
    void MakeItem();

    // ������ ���� (item create ��Ŷ�� �޾Ƽ� �ϴ� ��)
    void MakeItem(uint32_t i_id, float x, float y);

    // ������ ����
    void RemoveItem(uint32_t i_id);

    // �Ѿ� ����
    void RemoveBullet(uint32_t b_id);

    // �÷��̾� ������
    void PlayerDamage(float damage, bool dir, int32_t ClientID);

    // ���� ����
    void Gameover();

    // ��ũ�Ѹ��� ���� view ���� �Լ�
    void Scrolling(long long deltaTime);

    // ������ ��� ��ü�� �׸���
    virtual void draw();

    // ���� Bullet �߰�
    void AddEnemyBullet(float x, float y, bool direction, uint32_t type, uint32_t b_id);

    // ���� ��ü �ʱ�ȭ
    void InitText();
    void updateTexts();
    void drawTexts();

    void SetTitleFalse() { titleImage.SetShow(false); }

    Player& GetPlayer1() { return *player1; }
    Player& GetPlayer2() { return *player2; }
    Player& GetControlPlayer();
    Player& GetOtherPlayer();
};