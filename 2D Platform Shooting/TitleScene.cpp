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
    // �����찡 �����ִٸ� ���� ������ �ݺ��Ѵ�
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
    // ������ �ϴû����� ����
    window.clear(BackgroundColor);

    // TODO: �ϴ� ���⿡ �׸��� �׷�����
    Background.draw(window);

    // ���� �׸� ȭ������ �ٲٱ�
    window.display();
}

void TitleScene::handleInput()
{
    // Window ������ �� ExitŰ�� ������ �� ����
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