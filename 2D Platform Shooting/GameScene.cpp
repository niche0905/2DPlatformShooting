#include "pch.h"
#include "GameScene.h"
#include "Level.h"
#include "Gun.h"
#include "Item.h"
#include "Utilities.h"
#include "Image.h"

std::uniform_real_distribution<float> uid{ 0.0, 800.0 };

GameScene::GameScene() :
    window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "2D CLIENT"),
    level(),
    view(sf::FloatRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT)),
    Background{ TextureID::BACKGROUND, 3200, 3200 },
    UI{ {TextureID::UI1, 200, 150}, {TextureID::UI2, 200, 150} }
{
    makeTime = std::chrono::system_clock::now();
    players.emplace_back(100.0f, 400.0f, &level, TextureID::PLAYER1);
    players.emplace_back(500.0f, 400.0f, &level, sf::Keyboard::I, sf::Keyboard::K, sf::Keyboard::J, sf::Keyboard::L, sf::Keyboard::Slash, TextureID::PLAYER2);

    items.emplace_back(200.0f, -500.0f, &level);
    // 시작시 바로 플레어이 중앙으로 옮길지 말지
    //InitView();

    // CHAM_TODO: 일단 여기에 초기화.
    Background.SetPosition(-1200, -1200);
    UI[1].SetPosition(600, 0);

    InitText();
}

void GameScene::InitView()
{
    sf::Vector2f newPosition = players[0].getPosition();
    newPosition.y -= CameraOffset;

    view.setCenter(newPosition);
    window.setView(view);
}

void GameScene::run()
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

void GameScene::handleInput()
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
    }
}

void GameScene::update(long long deltaTime)
{
    // 모든 업데이트 해야할 항목을 업데이트
    for (Player& player : players)
        player.update(deltaTime);

    for (Item& item : items)
        item.update(deltaTime);

    // TODO: 아직 구현 안되었음
    bulletHit();

    // 플레이어 : 아이템 충돌 함수
    eatItem();

    std::chrono::system_clock::time_point nowTime = std::chrono::system_clock::now();

    auto timeDiff = std::chrono::duration_cast<std::chrono::milliseconds>(nowTime - makeTime);

    if (timeDiff.count() > 10000) {
        makeItem();
        makeTime = nowTime;
    }

    updateTexts();

    // 플레이어 위치를 기반으로 view를 설정하는 함수
    Scrolling(deltaTime);
}

void GameScene::bulletHit()
{
    for (Player& shooter : players) {
        for (Player& hitter : players) {
            if (&shooter == &hitter)
                continue;

            shooter.hitTheEnemy(hitter);
        }
    }
}

void GameScene::eatItem()
{
    for (Player& player : players) {
        for (auto iter{ items.begin() }; iter != items.end();) {
            if (iter->checkCollisionBullet(player.getGlobalBounds())) {
                iter = items.erase(iter);
                player.getItem();
                makeTime = std::chrono::system_clock::now();
            }
            else { ++iter; }
        }
    }
}

void GameScene::makeItem()
{
    float num = uid(RANDOM_ENGINE);
    items.emplace_back(num, -500.0f, &level);
}

void GameScene::Scrolling(long long deltaTime)
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

void GameScene::draw()
{
    // 윈도우 하늘색으로 리셋
    window.clear(BackgroundColor);

    // TODO: 일단 여기에 그림을 그려보자
    Background.draw(window);

    // 모든 객체 그리기
    level.draw(window);
    for (Item& item : items)
        item.draw(window);

    for (Player& player : players)
        player.draw(window);

    for (auto& obj : UI)
        obj.drawFixed(window);

    // 문자 그리기
    drawTexts();

    // 새로 그린 화면으로 바꾸기
    window.display();
}


void GameScene::InitText()
{
    if (not font.loadFromFile(FONT_PATH)) {
        exit(-1);
    }

    for (auto& text : texts) {
        text.setFont(font);
        text.setCharacterSize(20);
        text.setFillColor(sf::Color::Black);
    }

    texts[0].setString("Gun: ");
    texts[0].setPosition(30, 20);
    texts[1].setString("Remain: ");
    texts[1].setPosition(30, 60);
    texts[2].setString("Life: ");
    texts[2].setPosition(30, 100);
    texts[3].setString("Gun: ");
    texts[3].setPosition(630, 20);
    texts[4].setString("Remain: ");
    texts[4].setPosition(630, 60);
    texts[5].setString("Life: ");
    texts[5].setPosition(630, 100);
}

void GameScene::updateTexts()
{
    texts[0].setString("Gun: " + GunInfo.gun_table[players[0].getGunID()].name);
    if (auto val = players[0].getCurMag(); -1 == val) {
        texts[1].setString("Remain: INF");
    }
    else {
        texts[1].setString("Remain: " + std::to_string(val));
    }
    texts[2].setString("Life: " + std::to_string(players[0].getLife()));
    texts[3].setString("Gun: " + GunInfo.gun_table[players[1].getGunID()].name);
    if (auto val = players[1].getCurMag(); -1 == val) {
        texts[4].setString("Remain: INF");
    }
    else {
        texts[4].setString("Remain: " + std::to_string(val));
    }
    texts[5].setString("Life: " + std::to_string(players[1].getLife()));
}



void GameScene::drawTexts()
{
    sf::View currentView = window.getView();
    window.setView(window.getDefaultView());
    for (auto& obj : texts)
        window.draw(obj);
    window.setView(currentView);
}

//void Game::buildGun()
//{
//    loadGunFromFile(GunSavePath);
//}

//bool Game::loadGunFromFile(const std::string& filePath)
//{
//    std::ifstream inFile(filePath);
//    if (not inFile.is_open()) {
//        std::cerr << "Failed to open file: " << filePath << std::endl;
//        return false;
//    }
//
//    Gun gun;
//
//    // [cham] 9.22 
//    uint8 cnt{};
//    while (inFile >> gun)
//    {
//        gun.id = cnt;
//        g_guns[cnt++] = gun;
//    }
//
//    inFile.close();
//
//    return true;
//}

//void Game::saveGunFromFile(const std::string& filePath)
//{
//    std::ofstream outFile(filePath);
//    if (not outFile.is_open()) {
//        std::cerr << "Failed to open file: " << filePath << std::endl;
//        return;
//    }
//
//    // [cham] 9.22
//    for (const auto& gun : g_guns | std::views::values) {
//        outFile << gun << std::endl;
//    }
//
//    outFile.close();
//
//    return;
//}