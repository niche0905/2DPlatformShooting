#pragma once
#include "Scene.h"

class TitleScene : public Scene
{
private:
    sf::RenderWindow window;

    Image Background;
public:
    TitleScene();

    virtual void run();
    virtual void update(long long deltaTime);
    virtual void draw();
    virtual void handleInput();
};

