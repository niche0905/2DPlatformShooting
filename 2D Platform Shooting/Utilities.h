#pragma once

constexpr float GravityAcc = 9.810f;            // 중력 가속도
constexpr float GravityMul = 120.0f;            // 중력 가속도에 곱하는 수(게임성을 위해)
constexpr float PlatformUp = 10.0f;             // 플랫폼 위에 있다는 기준(충돌 처리 시 사용)

// 선형 보간
sf::Vector2f lerp(const sf::Vector2f& start, const sf::Vector2f& end, float t);

#define MAX_SIZE 64

#define SERVER_PORT 9000