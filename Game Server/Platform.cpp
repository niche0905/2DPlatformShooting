#include "pch.h"
#include "Platform.h"


Platform::Platform()
	: Object()
{
}

Platform::Platform(float pos_x, float pos_y, float width, float height)
	: Object(pos_x, pos_y, width, height, 0.0f, 0.0f)
{
}
