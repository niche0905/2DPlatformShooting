#pragma once

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.

#ifdef _DEBUG
#pragma comment(lib, "Debug\\Network Protocol.lib")
#else
#pragma comment(lib, "Release\\Network Protocol.lib")
#endif

#include "CorePch.h"
