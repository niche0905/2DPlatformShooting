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
    // �����찡 �����ִٸ� ���� ������ �ݺ��Ѵ�
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
    // Window ������ �� ExitŰ�� ������ �� ����
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
    // ��� ������Ʈ �ؾ��� �׸��� ������Ʈ
    player.update(deltaTime);
    dummy.update(deltaTime);

    // TODO: �Լ��� ó���ؾ� �� �� �ʹ�.
    player.hitTheEnemy(dummy);

    // �÷��̾� ��ġ�� ������� view�� �����ϴ� �Լ�
    Scrolling(deltaTime);
}

void Game::Scrolling(long long deltaTime)
{
    // Ÿ��( == �÷��̾�) ��ġ�� �˱�����
    sf::Vector2f targetPosition = player.getPosition();
    // ���� view�� ��ġ�� �˱�����
    sf::Vector2f currentPosition = view.getCenter();
    // view�� center�� �ణ �ø��� ����
    targetPosition.y -= CameraOffset;

    sf::Vector2f newPosition = lerp(currentPosition, targetPosition, CameraLagging * (deltaTime / 1000000.0f));

    view.setCenter(newPosition);
    window.setView(view);
}

void Game::draw()
{
    // ������ �ϴû����� ����
    window.clear(BackgroundColor);

    // ��� ��ü �׸���
    level.draw(window);
    player.draw(window);
    dummy.draw(window);

    // ���� �׸� ȭ������ �ٲٱ�
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