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

	// Ÿ��Ʋ �� ó��

	return scene;
}

std::shared_ptr<Scene> SceneManager::LoadGameScene()
{
	std::shared_ptr<Scene> scene = std::make_shared<Scene>();
	
	// ���� �� ó��
	
	return scene;
}
