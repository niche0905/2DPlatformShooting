#pragma once
#include "Scene.h"
#include "GameScene.h"
#include "TitleScene.h"
#include "pch.h"

class SceneManager {
	// ø÷ ΩÃ±€≈Ê¿Ã æ»µ«¡ˆ
	//DECLARE_SINGLE(SceneManager);

private:
	std::shared_ptr<Scene> activeScene;

public:
	SceneManager();
	~SceneManager();

	std::shared_ptr<Scene> LoadTitleScene();
	std::shared_ptr<Scene> LoadGameScene();

	std::shared_ptr<Scene> GetActiveScene() { return activeScene; }
};

