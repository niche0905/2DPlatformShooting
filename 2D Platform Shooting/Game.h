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
    // ������ ũ��� Ÿ��Ʋ �̸��� �����Ѵ�
    Game() : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "2D CLIENT"), level(), player(100.0f, 400.0f, level), view(sf::FloatRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT))
    {


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

    // ��ũ�Ѹ��� ���� view ���� �Լ�
    void Scrolling(long long deltaTime);

    // ������ ��� ��ü�� �׸���
    void draw();
};