#include "pch.h"
#include "SceneManager.h"

SceneManager::SceneManager()
{
	// 게임이 실행될 때 가장 먼저 로드되는 씬
	activeScene = LoadGameScene();
}

SceneManager::~SceneManager()
{

}

std::shared_ptr<Scene> SceneManager::LoadTitleScene()
{
	std::shared_ptr<TitleScene> scene = std::make_shared<TitleScene>();
	return scene;
}

std::shared_ptr<Scene> SceneManager::LoadGameScene()
{
	std::shared_ptr<GameScene> scene = std::make_shared<GameScene>();
	return scene;
}