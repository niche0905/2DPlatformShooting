#pragma once
#include "Level.h"
#include "Item.h"
#include "Image.h"

class Scene {
private:

protected:
    bool focus{ false };

public:
    virtual void run() {};
    virtual void update(long long deltaTime) {};
    virtual void draw() {};
    virtual void handleInput() {};

    bool GetFocus() { return focus; }
};