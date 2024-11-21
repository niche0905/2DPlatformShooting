#pragma once
#include "Scene.h"
#include "GameScene.h"
#include "TitleScene.h"

class SceneManager {

private:
	std::shared_ptr<Scene> activeScene;

public:
	SceneManager();
	~SceneManager();

	std::shared_ptr<Scene> LoadTitleScene();
	std::shared_ptr<Scene> LoadGameScene(uint32_t p_id);

	std::shared_ptr<Scene> GetActiveScene() const{ return activeScene; }
};

