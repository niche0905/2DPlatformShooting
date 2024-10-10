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

    // �ΰ�� �ڸ�Ʈ
    // Game Ŭ���� �ȿ� txt�� �ѵ� �޾Ƽ� guns�� �־��µ�
    // ������ �� �԰� �ٲٰ� �ϴ°� player�� �ϴ� ���ε�
    // �׷��� ���� guns�� Game�� ���� ���� Player Ŭ������ �ִ� �� �� ���ƺ��̴µ�
    // ��� ���� ���ʹ�

    // [cham] 9.22: 
    // gun�� ���� ������ unordered_map���� �������� �����ؼ� �������.
    // player�� gun�� id������ �������� �Ѵ�.
    // std::vector<Gun> guns;

    // �۽�ȣ �ڸ�Ʈ
    // Player�� ���� ��ü�� �ִ°� �ƴ϶� �ڷᱸ���� �־����
    // �׷��� �÷��̾���� �־ �÷��̾�� ���� �Ѿ� �浹 �˻縦 �ؾ���
    // �� �� PC���� �θ��̼� �ϴ°� ��ǥ�̸�

    sf::View view;

    std::chrono::system_clock::time_point makeTime;

public:
    // ������ ũ��� Ÿ��Ʋ �̸��� �����Ѵ�
    Game() : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "2D CLIENT"), level(), view(sf::FloatRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT))
    {
        makeTime = std::chrono::system_clock::now();
        players.emplace_back(100.0f, 400.0f, &level);
        players.emplace_back(500.0f, 400.0f, &level, sf::Keyboard::I, sf::Keyboard::K, sf::Keyboard::J, sf::Keyboard::L, sf::Keyboard::Enter);

        items.emplace_back(200.0f, -500.0f, &level);
        // ���۽� �ٷ� �÷����� �߾����� �ű��� ����
        //InitView();
    }

    // ī�޶� �並 Player�� �߽�����
    void InitView();

    // ������ �����Ű�� ���� ������ ������
    void run(); 

    // ���� Input�� ó���Ѵ�
    void handleInput();

    // ������ ��ü���� ������Ʈ�Ѵ�
    void update(long long deltaTime);

    // �Ѿ� �浹 ó�� (������ ������ Player)
    void bulletHit();

    // ������ �浹 ó��
    void eatItem();

    // ������ ����
    void makeItem();

    // ��ũ�Ѹ��� ���� view ���� �Լ�
    void Scrolling(long long deltaTime);

    // ������ ��� ��ü�� �׸���
    void draw();

    void buildGun();

    bool loadGunFromFile(const std::string& filePath);
    void saveGunFromFile(const std::string& filePath);
};