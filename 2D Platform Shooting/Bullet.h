#pragma once

constexpr float BulletWidth = 20.0f;
constexpr float BulletHeight = 6.0f;

// 총 종류에 따라 속도가 다를것이므로 주석 처리
// constexpr float BulletSpeed = 600.0f;
constexpr float BulletDamage = 10.0f;


class Bullet
{
private:
	bool direction;	// true : Left(←),	false : Right(→)
	float speed;
	float damage;

	sf::RectangleShape shape;

public:
	// 생성할 때 방향과 발사한 위치를 받아야 한다(+ 속도 추가 09/08 민경원)
	Bullet(bool direct, sf::Vector2f pos, float speed) : direction(direct), speed(speed), damage(BulletDamage)
	{
		shape.setSize(sf::Vector2f(BulletWidth, BulletHeight));
		shape.setOrigin(BulletWidth / 2, BulletHeight / 2);
		shape.setPosition(pos);
		shape.setFillColor(sf::Color::Yellow);
	}

	// (+ 데미지 추가 09/23 송승호)
	Bullet(bool direct, sf::Vector2f pos, float speed, float damage) : direction(direct), speed(speed), damage(damage)
	{
		shape.setSize(sf::Vector2f(BulletWidth, BulletHeight));
		shape.setOrigin(BulletWidth / 2, BulletHeight / 2);
		shape.setPosition(pos);
		shape.setFillColor(sf::Color::Yellow);
	}

	// 총알의 위치가 영역 밖으로 넘었는지를 판단한다
	bool isOutBounds(float left, float right);

	// 총알의 위치를 업데이트 한다
	void update(long long deltaTime);

	// 총알을 그려준다
	void draw(sf::RenderWindow& window) const;

	// 총알의 바운딩박스를 얻는다
	sf::FloatRect getGlobalBounds() const;

	// 방향을 알려준다
	bool getDirection() const;

	// 데미지를 알려준다.
	float getDamage() const;

};

