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
    // ������ ũ��� Ÿ��Ʋ �̸��� �����Ѵ�
    Game() : window(sf::VideoMode(800, 600), "2D CLIENT"), level(), player(100.0f, 400.0f, level) { }

    // ������ �����Ű�� ���� ������ ������
    void run(); 

    // ���� Input�� ó���Ѵ�
    void handleInput();

    // ������ ��ü���� ������Ʈ�Ѵ�
    void update(long long deltaTime);

    // ������ ��� ��ü�� �׸���
    void draw();
};