#pragma once
#include "Level.h"
#include "Item.h"
#include "Image.h"

class Scene {
private:
    
public:
    virtual void run() {};
    virtual void update(long long deltaTime) {};
    virtual void draw() {};
};