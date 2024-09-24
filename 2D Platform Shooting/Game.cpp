#include "Game.h"
#include "Utilities.h"

void Game::InitView()
{
    sf::Vector2f newPosition = players[0].getPosition();
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

        for (Player& player : players)
            player.handleInput(event);
        //dummy.handleInput(event);
    }
}

void Game::update(long long deltaTime)
{
    // 모든 업데이트 해야할 항목을 업데이트
    for (Player& player : players)
        player.update(deltaTime);

    for (Item& item : items)
        item.update(deltaTime);

    //dummy.update(deltaTime);

    // TODO: 아직 구현 안되었음
    bulletHit();

    // 플레이어 : 아이템 충돌 함수
    eatItem();

    //players[0].hitTheEnemy(dummy);

    // 플레이어 위치를 기반으로 view를 설정하는 함수
    Scrolling(deltaTime);
}

void Game::bulletHit()
{
    for (Player& shooter : players) {
        for (Player& hitter : players) {
            if (&shooter == &hitter)
                continue;

            shooter.hitTheEnemy(hitter);
        }
    }
}

void Game::eatItem()
{
    for (Player& player : players) {
        for (auto iter{ items.begin() }; iter != items.end();) {
            if (iter->checkCollisionBullet(player.getGlobalBounds())) {
                iter = items.erase(iter);
                makeItem();
                player.getItem();
            }
            else { ++iter; }
        }
    }
}

void Game::makeItem()
{
    items.emplace_back(200.0f, -500.0f, &level);
}

void Game::Scrolling(long long deltaTime)
{
    // 타겟( == 플레이어) 위치를 알기위한
    sf::Vector2f targetPosition = players[0].getPosition();
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
    for (Item& item : items)
        item.draw(window);

    for (Player& player : players)
        player.draw(window);

    //dummy.draw(window);

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