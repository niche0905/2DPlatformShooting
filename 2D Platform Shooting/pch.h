#pragma once

// SFML 라이브러리
#define SFML_STATIC
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#define _WINSOCK_DEPRECATED_NO_WARNINGS
// 윈속2 메인 헤더
#include <winsock2.h>
// 윈속2 확장 헤더
#include <ws2tcpip.h>

 // ws2_32.lib 링크
#pragma comment(lib, "ws2_32")
 
// 자료구조
#include <list>
#include <vector>
#include <string>
#include <array>
#include <queue>
#include <unordered_map>
#include <memory>

// 알고리즘
#include <random>
#include <chrono>
#include <ranges>
#include <algorithm>

// 입출력
#include <iostream>
#include <fstream>
#include <filesystem>
#include <print>

// 우리 라이브러리
#define WIN32_LEAN_AND_MEAN

#ifdef _DEBUG
#pragma comment(lib, "Debug\\Network Protocol.lib")
#else
#pragma comment(lib, "Release\\Network Protocol.lib")
#endif

#include "CorePch.h"

#include "GlobalVariable.h"

using std::cout;
using std::endl;
using namespace std::string_literals;
namespace fs = std::filesystem;

// type define
using int8 = __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;
using uint8 = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;

// constant
constexpr auto FONT_PATH{ "Resource\\Font\\malgun.ttf" };

// Scene 관련
constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;

const sf::Color BackgroundColor = sf::Color(135, 206, 235, 255);
const sf::Color PlatformColor = sf::Color(55, 55, 55, 255);

constexpr float CameraLagging = 10.0f;
constexpr float CameraOffset = 60.0f;