#include "pch.h"
#include "SceneManager.h"

SceneManager::SceneManager()
{
	activeScene = LoadGameScene();
}

SceneManager::~SceneManager()
{

}


void SceneManager::update()
{

}

void SceneManager::draw()
{

}

void SceneManager::LoadScene(std::wstring sceneName)
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