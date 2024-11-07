#pragma once

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.

#ifdef _DEBUG
#pragma comment(lib, "Debug\\Network Protocol.lib")
#else
#pragma comment(lib, "Release\\Network Protocol.lib")
#endif

#include "CorePch.h"

// Window 관련 헤더
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32")

// 표준 라이브러리
#include <list>
#include <vector>
#include <string>
#include <array>
#include <queue>
#include <unordered_map>

#include <random>
#include <chrono>
#include <ranges>
#include <algorithm>

#include <iostream>
#include <fstream>
#include <filesystem>

// 출력을 편하게.
using std::cout;
using std::endl;

// type alias
using int8 = __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;
using uint8 = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;