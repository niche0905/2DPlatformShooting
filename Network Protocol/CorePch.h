#pragma once

#include "Packet.h"
#include "GunLoader.h"
#include "Platfoms.h"
#include "TimerManager.h"

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32")

#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

#include <iostream>
#include <random>

extern std::default_random_engine RANDOM_ENGINE;