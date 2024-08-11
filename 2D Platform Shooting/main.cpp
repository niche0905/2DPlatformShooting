#define SFML_STATIC
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <chrono>

class Player {
private:
    sf::RectangleShape shape;
    sf::Vector2f velocity;
    float speed;
    bool isJumping;
    float jumpHeight;
    sf::RenderWindow window;

public:
    Player(float x, float y) : speed(500.0f), isJumping(false), jumpHeight(750.0f) {
        shape.setSize(sf::Vector2f(50.0f, 50.0f));
        shape.setPosition(x, y);
        shape.setFillColor(sf::Color::Green);
    }

    void update(long long dt) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            velocity.x = -speed;
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            velocity.x = speed;
        }
        else {
            velocity.x = 0.0f;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            window.close();
        }

        if (isJumping) {
            velocity.y += 0.9810f; // 중력 가속도 적용
            if (shape.getPosition().y >= 300.0f)
            {
                isJumping = false;
                velocity.y = 0.0f;
                shape.setPosition(shape.getPosition().x, 300.0f);
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !isJumping) {
            velocity.y = -2.0f * 0.9810f * jumpHeight; // 중력 가속도를 고려한 점프 초기 속도
            isJumping = true;
        }

        shape.move(velocity * (dt / 1000000000.0f)); // 시간 간격을 고려한 이동
    }

    void draw(sf::RenderWindow& window) {
        window.draw(shape);
    }

    sf::FloatRect getGlobalBounds() const {
        return shape.getGlobalBounds();
    }

    void handleCollision(sf::FloatRect other) {
        sf::FloatRect playerBounds = shape.getGlobalBounds();
        if (playerBounds.intersects(other)) {
            isJumping = false; // 충돌 시 점프 상태 초기화
        }
    }
};

class Game {
private:
    sf::RenderWindow window;
    Player player;
    std::chrono::time_point<std::chrono::high_resolution_clock> lastTime = std::chrono::high_resolution_clock::now();

public:
    Game() : window(sf::VideoMode(800, 600), "2D CLIENT"), player(100.0f, 300.0f) {}

    void run() {
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
            }
            auto nowTime = std::chrono::high_resolution_clock::now();
            auto deltaTime = std::chrono::duration_cast<std::chrono::nanoseconds>(nowTime - lastTime).count();
            player.update(deltaTime);
            lastTime = nowTime;

            window.clear(sf::Color::White);
            player.draw(window);
            window.display();
        }
    }
};

int main() {
    Game game;
    game.run();

    return 0;
}
