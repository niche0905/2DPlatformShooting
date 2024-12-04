#pragma once

constexpr float GravityAcc = 9.810f;            // �߷� ���ӵ�
constexpr float GravityMul = 120.0f;            // �߷� ���ӵ��� ���ϴ� ��(���Ӽ��� ����)
constexpr float PlatformUp = 10.0f;             // �÷��� ���� �ִٴ� ����(�浹 ó�� �� ���)

// ���� ����
sf::Vector2f lerp(const sf::Vector2f& start, const sf::Vector2f& end, float t);

#define MAX_SIZE 64

#define SERVER_PORT 9000