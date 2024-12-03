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
    UI{ {TextureID::UI1, 200, 150},
        {TextureID::UI2, 200, 150}},
    titleImage{TextureID::TITLE, WINDOW_WIDTH , WINDOW_HEIGHT},
    resultImage{ {TextureID::P1WIN, WINDOW_WIDTH, WINDOW_HEIGHT},
                {TextureID::P2WIN, WINDOW_WIDTH, WINDOW_HEIGHT} }
{
    makeTime = std::chrono::system_clock::now();

    player1 = new Player{ 100.0f, 400.0f, &level, TextureID::PLAYER1, 0 };
    player2 = new Player{ 400.0f, 400.0f, &level, TextureID::PLAYER2, 1 };

    //items.emplace_back(200.0f, -500.0f, &level);
    // ���۽� �ٷ� �÷����� �߾����� �ű��� ����
    //InitView();

    Background.SetPosition(-1200, -1200);
    UI[1].SetPosition(600, 0);
    for (auto& img : resultImage) {
        img.SetShow(false);
    }

    InitText();

    timer.Init();
}

void GameScene::InitView()
{
    sf::Vector2f newPosition = player1->getPosition();
    newPosition.y -= CameraOffset;

    view.setCenter(newPosition);
    window.setView(view);
}

void GameScene::run()
{
    // �����찡 �����ִٸ� ���� ������ �ݺ��Ѵ�
    while (window.isOpen()) {
        handleInput();

        timer.Update();
        //sf::Time dtSFTime = clock.restart();
        //long long deltaTime = dtSFTime.asMicroseconds();

        //update(deltaTime);
        update(timer.getDeltaTime());

        draw();
    }
}

void GameScene::handleInput()
{
    // Window ������ �� ExitŰ�� ������ �� ����
    sf::Event event;

    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        if (event.type == sf::Event::KeyPressed and window.hasFocus() and event.key.code == sf::Keyboard::Escape) {
            window.close();
        }

        // TODO : �������̶�� ��ġ����ŷ ��Ŷ ���� �� ���� �ϱ�
        if (event.type == sf::Event::KeyPressed and window.hasFocus() and event.key.code == sf::Keyboard::R and (not match)) {
            auto buf = myNP::CS_MATCHMAKING_PACKET::MakePacket();
            network_mgr.SendPacket(
                reinterpret_cast<char*>(&buf),
                myNP::CS_MATCHMAKING
            );
            match = true;

            cout << "Match gogo\n";
            WaitForSingleObject(network_mgr.GetProcessEvent(), WSA_INFINITE);

            network_mgr.ProcessPacket();

            SetEvent(network_mgr.GetRecvEvent());
        }

        // TODO : �Ʒ� �ڵ� ������ 2�� ������ ���ľ� �� ����
        //        �������� ���� �ڵ� ��ǲ �ް� �ϰ� (�÷��̾)
        if (window.hasFocus()) {
            focus = true;
            player1->handleInput(event);
            player2->handleInput(event);
        }
        else {
            focus = false;
        }
    }
}

void GameScene::updateEnemyBullets(long long deltaTime)
{
    for (auto it = enemy_bullets.begin(); it != enemy_bullets.end(); ) {
        it->update(deltaTime);
        if (it->isOutBounds(level.leftBound - 1000.0f, level.rightBound + 1000.0f))
            it = enemy_bullets.erase(it);
        else
            ++it;
    }
}

void GameScene::update(long long deltaTime)
{
    //network_mgr.Update();

    // ��� ������Ʈ �ؾ��� �׸��� ������Ʈ
    player1->update(deltaTime);
    player2->update(deltaTime);

    // REMOVE:
    // �ӽ÷� Ȯ���� �� �ְ� ��.
    if (0 == player1->getLife()) {
        resultImage[1].SetShow(true);
    }

    if (0 == player2->getLife()) {
        resultImage[0].SetShow(true);
    }

    for (Item& item : items)
        item.update(deltaTime);

    updateEnemyBullets(deltaTime);

    updateTexts();

    // �÷��̾� ��ġ�� ������� view�� �����ϴ� �Լ�
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

void GameScene::MakeItem()
{
    float num = uid(RANDOM_ENGINE);
    items.emplace_back(0, num, -500.0f, &level);
}

void GameScene::MakeItem(uint32_t i_id, float x, float y)
{
    items.emplace_back(i_id, x, y, &level);
}

void GameScene::RemoveItem(uint32_t i_id)
{
    items.remove_if([i_id](const Item& item) {
        return item.GetItemId() == i_id;
        });
}

void GameScene::RemoveBullet(uint32_t b_id)
{
    enemy_bullets.remove_if([b_id](const Bullet& bullet) {
        return bullet.GetBulletId() == b_id;
        });
}

void GameScene::PlayerDamage(float damage, int32_t ClientID)
{
    if (ClientID == 0) player1->takeDamage(player1->getDirection(), damage);
    else player2->takeDamage(player2->getDirection(), damage);
}

void GameScene::Gameover()
{
    // ���� ����
   
    // ���� ���� UI ����

    if (0 == player1->getLife()) {
        resultImage[1].SetShow(true);
    }

    if (0 == player2->getLife()) {
        resultImage[0].SetShow(true);
    }

    // �� �Ŀ� �ٽ� Title UI ����

}

void GameScene::Scrolling(long long deltaTime)
{
    // Ÿ��( == �÷��̾�) ��ġ�� �˱�����
    sf::Vector2f targetPosition = player1->getPosition();
    if (network_mgr.GetClientID() == 1) {
        targetPosition = player2->getPosition();
    }
    // ���� view�� ��ġ�� �˱�����
    sf::Vector2f currentPosition = view.getCenter();
    // view�� center�� �ణ �ø��� ����
    targetPosition.y -= CameraOffset;

    sf::Vector2f newPosition = lerp(currentPosition, targetPosition, CameraLagging * (deltaTime / myNP::microToSecond));

    view.setCenter(newPosition);
    window.setView(view);
}

void GameScene::draw()
{
    // ������ �ϴû����� ����
    window.clear(BackgroundColor);

    // ������ �׸���
    Background.draw(window);

    // ��� ��ü �׸���
    level.draw(window);
    for (Item& item : items)
        item.draw(window);

    player1->draw(window);
    player2->draw(window);

    for (auto& obj : UI)
        obj.drawFixed(window);

    titleImage.drawFixed(window);

    for (auto& obj : resultImage)
        obj.drawFixed(window);

    for (auto& enemy_bullet : enemy_bullets)
        enemy_bullet.draw(window);



    // ���� �׸���
    drawTexts();

    // ���� �׸� ȭ������ �ٲٱ�
    window.display();
}

void GameScene::AddEnemyBullet(float x, float y, bool direction, uint32_t type, uint32_t b_id)
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
    texts[0].setString("Gun: " + GunLoader::Instance().GetGunTable()[player1->getGunID()].name);
    if (auto val = player1->getCurMag(); -1 == val) {
        texts[1].setString("Remain: INF");
    }
    else {
        texts[1].setString("Remain: " + std::to_string(val));
    }
    texts[2].setString("Life: " + std::to_string(player1->getLife()));
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

Player& GameScene::GetControlPlayer()
{
    if (network_mgr.GetClientID() == 0) {
        return  *player1;
    }
    else {
        return  *player2;
    }
}

Player& GameScene::GetOtherPlayer()
{
    if (network_mgr.GetClientID() == 0) {
        return  *player2;
    }
    else {
        return  *player1;
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