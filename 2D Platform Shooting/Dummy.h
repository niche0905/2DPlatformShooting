#pragma once

// Dummy ��ü ����(�۽�ȣ 09/03)
class Dummy {
    bool isActive;

    sf::RectangleShape shape;
    sf::Vector2f velocity;

    float damaged;      // ���� ���� �� -(����) �������� ���� ���� +(���) ���������� ���� ����

    bool direction;     // true : Left(��),	false : Right(��)
    float width;
    float height;
    float speed;

    float jumpHeight;
    int jumpChance;     // ���� ��ȸ
    int maxJumpChance;  // �ִ� ���� ��ȸ (�ʱ�ȭ �� �� ���)
    bool OnAir;
    int life;

    uint8_t gunId;

    std::list<Bullet> bullets;
    int curMag;
    std::chrono::system_clock::time_point lastFireTime;

    sf::RenderWindow window;
    Level* level;
    Image image{ TextureID::BANANA };

public:
    Dummy() = default;

    Dummy(float x, float y, Level* level, int texture_id);

    // ������ Input�� ó���Ѵ�
    void handleInput(const sf::Event& event);

    // ���̰� ���� �߻��Ѵ�.
    void fireBullet();

    // ������ ��ġ�� ������Ʈ�Ѵ�
    void update(long long deltaTime);

    // ���̰� �߻��� �Ѿ� ������Ʈ
    void updateBullets(long long deltaTime);

    // ���̸� �׸���
    void draw(sf::RenderWindow& window);

    // ���忡���� �ٿ���ڽ��� ��´�
    sf::FloatRect getGlobalBounds() const;

    // �浹üũ�� �Ѵ�
    bool checkCollision(sf::FloatRect other);

    // ������ ��ġ(�Ǻ� ��)�� ã�� �Լ�
    sf::Vector2f getPosition() const;

    // �Ѿ� �浹üũ(������ �浹üũ�� �ٸ��� �浹 ������ ó��)
    bool checkCollisionBullet(sf::FloatRect other);

    // ���̿��� �������� �ش�
    void takeDamage(bool direction, float damage);

    // ���ط��� �������ش�(������Ʈ)
    void damageControll(long long deltaTime);

    void setPosition(float x, float y);

    void getItem();

    // Getter
    int getCurMag() const { return curMag; }
    int getGunID() const { return gunId; }
    int getLife() const { return life; }

};