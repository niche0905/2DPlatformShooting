#include "Game.h"


void Game::run()
{
    while (window.isOpen()) {
        handleInput();

        sf::Time dtSFTime = clock.restart();
        long long deltaTime = dtSFTime.asMicroseconds();

        update(deltaTime);

        draw();
    }
}

void Game::handleInput()
{
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
    }
}

void Game::update(long long deltaTime)
{
    player.update(deltaTime);
}

void Game::draw()
{
    window.clear(sf::Color::White);

    player.draw(window);
    level.draw(window);

    window.display();
}
