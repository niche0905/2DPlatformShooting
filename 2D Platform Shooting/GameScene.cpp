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
    level{},
    view(sf::FloatRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT)),
    Background{ TextureID::BACKGROUND, 3200, 3200 },
    UI{ {TextureID::UI1, 200, 150}, {TextureID::UI2, 200, 150}}
{
    makeTime = std::chrono::system_clock::now();

    // 1p

    player = new Player{ 100.0f, 400.0f, &level, TextureID::PLAYER1, 0};
    player2 = new Player{ 400.0f, 400.0f, &level, TextureID::PLAYER2, 1};
   

    // items.emplace_back(200.0f, -500.0f, &level);
    // 시작시 바로 플레어이 중앙으로 옮길지 말지
    //InitView();

    Background.SetPosition(-1200, -1200);
    UI[1].SetPosition(600, 0);

    InitText();
}

void GameScene::InitView()
{
    sf::Vector2f newPosition = player->getPosition();
    newPosition.y -= CameraOffset;

    view.setCenter(newPosition);
    window.setView(view);
}

void GameScene::run()
{
    // 윈도우가 열려있다면 게임 루프를 반복한다
    sf::Time dtSFTime = clock.restart();
    while (window.isOpen()) {
        handleInput();

        long long deltaTime = dtSFTime.asMicroseconds();

        update(deltaTime);

        draw();

        dtSFTime = clock.restart();
        if (true == network_mgr.getSendMove()) {
            WaitForSingleObject(network_mgr.GetRecvEvent(), INFINITE);
            network_mgr.send_move = false;
            network_mgr.setSendMove(false);
            timer.Init();
            clock.restart();
        }
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

        if (event.key.code == sf::Keyboard::R) {
            // revivePlayer();     // 임시로 키 바인딩으로 부활 호출
            auto buf = myNP::CS_MATCHMAKING_PACKET::MakePacket();
            network_mgr.SendPacket(
                reinterpret_cast<char*>(&buf),
                myNP::CS_MATCHMAKING
            );
        }
        player->handleInput(event);
        player2->handleInput(event);
    }
}

void GameScene::update(long long deltaTime)
{
    //network_mgr.Update();

    // 모든 업데이트 해야할 항목을 업데이트
    player2->update(deltaTime);
    player->update(deltaTime);
    // player2->update(deltaTime);

    for (Item& item : items)
        item.update(deltaTime);

    //bulletHit();

    //eatItem();

    std::chrono::system_clock::time_point nowTime = std::chrono::system_clock::now();

    auto timeDiff = std::chrono::duration_cast<std::chrono::milliseconds>(nowTime - makeTime);

    updateTexts();

    // 플레이어 위치를 기반으로 view를 설정하는 함수
    Scrolling(deltaTime);


}

//void GameScene::bulletHit()
//{
//    for (Player& shooter : players) {
//        for (Player& hitter : players) {
//            if (&shooter == &hitter)
//                continue;
//
//            shooter.hitTheEnemy(hitter);
//        }
//    }
//}
//

//void GameScene::eatItem()
//{
//    for (Player& player : players) {
//        for (auto iter{ items.begin() }; iter != items.end();) {
//            if (iter->checkCollisionBullet(player->getGlobalBounds())) {
//                iter = items.erase(iter);
//                player->getItem();
//                makeTime = std::chrono::system_clock::now();
//            }
//            else { ++iter; }
//        }
//    }
//}

void GameScene::Scrolling(long long deltaTime)
{
    // 타겟( == 플레이어) 위치를 알기위한
    sf::Vector2f targetPosition = player->getPosition();
    if (network_mgr.GetClientId() == 1) {
        targetPosition = player2->getPosition();
    }

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

    player->draw(window);
    player2->draw(window);

    for (auto& obj : UI)
        obj.drawFixed(window);

    // 문자 그리기
    drawTexts();

    // 새로 그린 화면으로 바꾸기
    window.display();
}

void GameScene::AddEnemyBullet(float x, float y, bool direction, uint32_t type)
{
    enemy_bullets.emplace_back(direction, x, y, GunLoader::Instance().GetGunTable()[type].speed);
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
    texts[0].setString("Gun: " + GunLoader::Instance().GetGunTable()[player->getGunID()].name);
    if (auto val = player->getCurMag(); -1 == val) {
        texts[1].setString("Remain: INF");
    }
    else {
        texts[1].setString("Remain: " + std::to_string(val));
    }
    texts[2].setString("Life: " + std::to_string(player->getLife()));
    texts[3].setString("Gun: " + GunLoader::Instance().GetGunTable()[player2->getGunID()].name);
    if (auto val = player2->getCurMag(); -1 == val) {
        texts[4].setString("Remain: INF");
    }
    else {
        texts[4].setString("Remain: " + std::to_string(val));
    }
    texts[5].setString("Life: " + std::to_string(player2->getLife()));
}

void GameScene::drawTexts()
{
    sf::View currentView = window.getView();
    window.setView(window.getDefaultView());
    for (auto& obj : texts)
        window.draw(obj);
    window.setView(currentView);
}

Player& GameScene::GetOtherPlayer()
{
    if (network_mgr.GetClientId() == 0) {
        return *player2;
    }
    else {
        return *player;
    }

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