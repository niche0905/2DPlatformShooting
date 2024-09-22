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
    buildGun();
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

        player.handleInput(event);
        dummy.handleInput(event);
    }
}

void Game::update(long long deltaTime)
{
    // 모든 업데이트 해야할 항목을 업데이트
    player.update(deltaTime);
    dummy.update(deltaTime);

    // TODO: 함수로 처리해야 할 듯 싶다.
    player.hitTheEnemy(dummy);

    // 플레이어 위치를 기반으로 view를 설정하는 함수
    Scrolling(deltaTime);
}

void Game::Scrolling(long long deltaTime)
{
    // 타겟( == 플레이어) 위치를 알기위한
    sf::Vector2f targetPosition = player.getPosition();
    // 지금 view의 위치를 알기위한
    sf::Vector2f currentPosition = view.getCenter();
    // view의 center를 약간 올리기 위해
    targetPosition.y -= CameraOffset;

    sf::Vector2f newPosition = lerp(currentPosition, targetPosition, CameraLagging * (deltaTime / 1000000.0f));

    view.setCenter(newPosition);
    window.setView(view);
}

void Game::draw()
{
    // 윈도우 하늘색으로 리셋
    window.clear(BackgroundColor);

    // 모든 객체 그리기
    level.draw(window);
    player.draw(window);
    dummy.draw(window);

    // 새로 그린 화면으로 바꾸기
    window.display();
}

void Game::buildGun()
{
    loadGunFromFile(GunSavePath);
}

bool Game::loadGunFromFile(const std::string& filePath)
{
    std::ifstream inFile(filePath);
    if (not inFile.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return false;
    }

    Gun gun;

    // [cham] 9.22 
    uint8_t cnt{};
    while (inFile >> gun)
    {
        g_guns[cnt++] = gun;
    }

    inFile.close();

    return true;
}

void Game::saveGunFromFile(const std::string& filePath)
{
    std::ofstream outFile(filePath);
    if (not outFile.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return;
    }

    // [cham] 9.22
    for (const auto& gun : g_guns | std::views::values) {
        outFile << gun << std::endl;
    }

    outFile.close();

    return;
}