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
            // TEMP : 테스트용 수정 (11/15 송승호)

            myNP::CS_MOVE_PACKET packet = myNP::CS_MOVE_PACKET::MakePacket(12, 0.5f, 119.0f, true);
            //packet.ntohByteOrder();
            std::cout << sizeof(packet) << std::endl;
            std::cout << static_cast<int>(packet.size) << std::endl;
            std::cout << static_cast<int>(packet.id) << std::endl;
            std::cout << ntohl(packet.p_id) << std::endl;
            std::cout << ntohf(packet.posX) << std::endl;
            std::cout << ntohf(packet.posY) << std::endl;
            std::cout << packet.dir << std::endl;
            std::cout << "패킷을 보낸다" << std::endl;
            network_mgr.SendPacket(reinterpret_cast<char*>(&packet), myNP::CS_MOVE);
        }
        if (event.type == sf::Event::KeyPressed and event.key.code == sf::Keyboard::Escape) {
            window.close();
        }

    }
}