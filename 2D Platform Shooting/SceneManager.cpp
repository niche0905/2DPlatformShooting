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
	std::shared_ptr<Scene> scene = std::make_shared<Scene>();


	return scene;
}

std::shared_ptr<Scene> SceneManager::LoadGameScene()
{
	std::shared_ptr<Scene> scene = std::make_shared<Scene>();


	return scene;
}