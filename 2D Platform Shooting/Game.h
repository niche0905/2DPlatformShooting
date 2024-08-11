#pragma once
#define SFML_STATIC
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <chrono>
#include "Player.h"


class Game {
private:
    sf::RenderWindow window;
    Player player;
    std::chrono::time_point<std::chrono::high_resolution_clock> lastTime = std::chrono::high_resolution_clock::now();

public:
    Game() : window(sf::VideoMode(800, 600), "2D CLIENT"), player(100.0f, 300.0f) {}

    void run() {
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
            }
            auto nowTime = std::chrono::high_resolution_clock::now();
            auto deltaTime = std::chrono::duration_cast<std::chrono::nanoseconds>(nowTime - lastTime).count();
            player.update(deltaTime);
            lastTime = nowTime;

            window.clear(sf::Color::White);
            player.draw(window);
            window.display();
        }
    }
};