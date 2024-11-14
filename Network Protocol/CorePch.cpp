#include "pch.h"
#include "CorePch.h"
#include <random>


std::random_device rd{};
std::default_random_engine RANDOM_ENGINE{ rd() };