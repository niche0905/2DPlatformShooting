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
    Game() : window(sf::VideoMode(800, 600), "2D CLIENT"), level(), player(100.0f, 400.0f, level) { }

    void run(); 

    void handleInput();

    void update(long long deltaTime);

    void draw();
};