#pragma once

// SFML 라이브러리
#define SFML_STATIC
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

// 표준 라이브러리
#include <Windows.h>
 
// 자료구조
#include <list>
#include <vector>
#include <string>
#include <array>
#include <queue>
#include <unordered_map>

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


extern std::default_random_engine RANDOM_ENGINE;