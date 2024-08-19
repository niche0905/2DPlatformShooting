#include "Game.h"
#include "Utilities.h"


void Game::InitView()
{
    sf::Vector2f newPosition = player.getPosition();
    newPosition.y -= CameraOffset;

    view.setCenter(newPosition);
    window.setView(view);
}

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
        if (event.type == sf::Event::KeyPressed and event.key.code == sf::Keyboard::Escape) {
            window.close();
        }
    }
}

void Game::update(long long deltaTime)
{
    // ��� ������Ʈ �ؾ��� �׸��� ������Ʈ
    player.update(deltaTime);

    // �÷��̾� ��ġ�� ������� view�� �����ϴ� �Լ�
    Scrolling();
}

void Game::Scrolling()
{
    // Ÿ��( == �÷��̾�) ��ġ�� �˱�����
    sf::Vector2f targetPosition = player.getPosition();
    // ���� view�� ��ġ�� �˱�����
    sf::Vector2f currentPosition = view.getCenter();
    // view�� center�� �ణ �ø��� ����
    targetPosition.y -= CameraOffset;

    sf::Vector2f newPosition = lerp(currentPosition, targetPosition, CameraLagging);

    view.setCenter(newPosition);
    window.setView(view);
}

void Game::draw()
{
    // ������ �Ͼ������ ����
    window.clear(sf::Color::White);

    // ��� ��ü �׸���
    level.draw(window);
    player.draw(window);

    // ���� �׸� ȭ������ �ٲٱ�
    window.display();
}
