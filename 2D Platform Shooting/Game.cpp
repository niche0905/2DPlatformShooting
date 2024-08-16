#include "Game.h"


void Game::run()
{
    // �����찡 �����ִٸ� ���� ������ �ݺ��Ѵ�
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
    // Window ������ �� ExitŰ�� ������ �� ����
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
    }
}

void Game::update(long long deltaTime)
{
    // ��� ������Ʈ �ؾ��� �׸��� ������Ʈ
    player.update(deltaTime);
}

void Game::draw()
{
    // ������ �Ͼ������ ����
    window.clear(sf::Color::White);

    // ��� ��ü �׸���
    player.draw(window);
    level.draw(window);

    // ���� �׸� ȭ������ �ٲٱ�
    window.display();
}
