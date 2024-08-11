#pragma once
#define SFML_STATIC
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <chrono>
#include "Player.h"
#include "Level.h"


class Game {
private:
    sf::RenderWindow window;
    Player player;
    Level level;
    std::chrono::time_point<std::chrono::high_resolution_clock> lastTime = std::chrono::high_resolution_clock::now();

public:
    Game() : window(sf::VideoMode(800, 600), "2D CLIENT"), player(100.0f, 350.0f), level() {}

    void run(); 
};