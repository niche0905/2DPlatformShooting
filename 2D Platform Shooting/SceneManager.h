#pragma once
#include "Scene.h"
#include "pch.h"

class SceneManager {
private:
	std::shared_ptr<Scene> activeScene;

public:
	SceneManager();
	~SceneManager();

	void update();
	void draw();
	void LoadScene(std::wstring sceneName);

	std::shared_ptr<Scene> LoadTitleScene();
	std::shared_ptr<Scene> LoadGameScene();

	std::shared_ptr<Scene> GetActiveScene() { return activeScene; }
};

