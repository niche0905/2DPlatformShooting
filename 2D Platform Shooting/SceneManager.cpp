#include "pch.h"
#include "SceneManager.h"

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

	// 타이틀 씬 처리

	return scene;
}

std::shared_ptr<Scene> SceneManager::LoadGameScene()
{
	std::shared_ptr<Scene> scene = std::make_shared<Scene>();
	
	// 게임 씬 처리
	
	return scene;
}
