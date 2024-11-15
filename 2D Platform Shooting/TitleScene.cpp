#include "pch.h"
#include "TitleScene.h"

TitleScene::TitleScene() :
	window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "2D CLIENT"),
	Background{ TextureID::BACKGROUND, 3200, 3200 }
{
	Background.SetPosition(-1200, -1200);
}

void TitleScene::run()
{
    // 윈도우가 열려있다면 게임 루프를 반복한다
    while (window.isOpen()) {
        handleInput();

        draw();
    }
}

void TitleScene::update(long long deltaTime)
{

}

void TitleScene::draw()
{
    // 윈도우 하늘색으로 리셋
    window.clear(BackgroundColor);

    // TODO: 일단 여기에 그림을 그려보자
    Background.draw(window);

    // 새로 그린 화면으로 바꾸기
    window.display();
}

void TitleScene::handleInput()
{
    // Window 오른쪽 위 Exit키를 눌렀을 때 종료
    sf::Event event;

    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        if (event.type == sf::Event::MouseButtonPressed and event.key.code == sf::Mouse::Left) {
            
        }
        if (event.type == sf::Event::KeyPressed and event.key.code == sf::Keyboard::Escape) {
            window.close();
        }

    }
}