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
    // 윈도우가 열려있다면 게임 루프를 반복한다
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
    // Window 오른쪽 위 Exit키를 눌렀을 때 종료
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
    // 모든 업데이트 해야할 항목을 업데이트
    player.update(deltaTime);

    // 플레이어 위치를 기반으로 view를 설정하는 함수
    Scrolling();
}

void Game::Scrolling()
{
    // 타겟( == 플레이어) 위치를 알기위한
    sf::Vector2f targetPosition = player.getPosition();
    // 지금 view의 위치를 알기위한
    sf::Vector2f currentPosition = view.getCenter();
    // view의 center를 약간 올리기 위해
    targetPosition.y -= CameraOffset;

    sf::Vector2f newPosition = lerp(currentPosition, targetPosition, CameraLagging);

    view.setCenter(newPosition);
    window.setView(view);
}

void Game::draw()
{
    // 윈도우 하얀색으로 리셋
    window.clear(sf::Color::White);

    // 모든 객체 그리기
    level.draw(window);
    player.draw(window);

    // 새로 그린 화면으로 바꾸기
    window.display();
}
