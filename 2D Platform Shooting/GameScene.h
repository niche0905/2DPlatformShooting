#pragma once
#include "Level.h"
#include "Player.h"
#include "Item.h"
#include "Image.h"
#include "Scene.h"

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;

const sf::Color BackgroundColor = sf::Color(135, 206, 235, 255);

constexpr float CameraLagging = 10.0f;
constexpr float CameraOffset = 60.0f;

class GameScene : public Scene {
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
    // ������ ũ��� Ÿ��Ʋ �̸��� �����Ѵ�
    GameScene();

    // ī�޶� �並 Player�� �߽�����
    void InitView();

    // ������ �����Ű�� ���� ������ ������
    virtual void run();

    // ���� Input�� ó���Ѵ�
    void handleInput();

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
};