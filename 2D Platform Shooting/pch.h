#pragma once

// SFML ���̺귯��
#define SFML_STATIC
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

// ����2 ���� ���
#include <winsock2.h>
// ����2 Ȯ�� ���
#include <ws2tcpip.h>

 // ws2_32.lib ��ũ
#pragma comment(lib, "ws2_32")
 
// �ڷᱸ��
#include <list>
#include <vector>
#include <string>
#include <array>
#include <queue>
#include <unordered_map>

// �˰���
#include <random>
#include <chrono>
#include <ranges>
#include <algorithm>

// �����
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