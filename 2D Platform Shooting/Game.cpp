#include "Game.h"


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
    }
}

void Game::update(long long deltaTime)
{
    // 모든 업데이트 해야할 항목을 업데이트
    player.update(deltaTime);
}

void Game::draw()
{
    // 윈도우 하얀색으로 리셋
    window.clear(sf::Color::White);

    // 모든 객체 그리기
    player.draw(window);
    level.draw(window);

    // 새로 그린 화면으로 바꾸기
    window.display();
}
