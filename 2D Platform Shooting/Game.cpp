#include "Game.h"


void Game::run()
{
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
        level.draw(window);

        window.display();
    }
}