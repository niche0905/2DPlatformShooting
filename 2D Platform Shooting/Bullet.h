#pragma once

constexpr float BulletWidth = 20.0f;
constexpr float BulletHeight = 6.0f;

// �� ������ ���� �ӵ��� �ٸ����̹Ƿ� �ּ� ó��
// constexpr float BulletSpeed = 600.0f;
constexpr float BulletDamage = 10.0f;


class Bullet
{
private:
	bool direction;	// true : Left(��),	false : Right(��)
	float speed;
	float damage;

	sf::RectangleShape shape;

public:
	// ������ �� ����� �߻��� ��ġ�� �޾ƾ� �Ѵ�(+ �ӵ� �߰� 09/08 �ΰ��)
	Bullet(bool direct, sf::Vector2f pos, float speed) : direction(direct), speed(speed), damage(BulletDamage)
	{
		shape.setSize(sf::Vector2f(BulletWidth, BulletHeight));
		shape.setOrigin(BulletWidth / 2, BulletHeight / 2);
		shape.setPosition(pos);
		shape.setFillColor(sf::Color::Yellow);
	}

	// (+ ������ �߰� 09/23 �۽�ȣ)
	Bullet(bool direct, sf::Vector2f pos, float speed, float damage) : direction(direct), speed(speed), damage(damage)
	{
		shape.setSize(sf::Vector2f(BulletWidth, BulletHeight));
		shape.setOrigin(BulletWidth / 2, BulletHeight / 2);
		shape.setPosition(pos);
		shape.setFillColor(sf::Color::Yellow);
	}

	// �Ѿ��� ��ġ�� ���� ������ �Ѿ������� �Ǵ��Ѵ�
	bool isOutBounds(float left, float right);

	// �Ѿ��� ��ġ�� ������Ʈ �Ѵ�
	void update(long long deltaTime);

	// �Ѿ��� �׷��ش�
	void draw(sf::RenderWindow& window) const;

	// �Ѿ��� �ٿ���ڽ��� ��´�
	sf::FloatRect getGlobalBounds() const;

	// ������ �˷��ش�
	bool getDirection() const;

	// �������� �˷��ش�.
	float getDamage() const;

};

