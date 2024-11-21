#include "pch.h"
#include "SceneManager.h"

SceneManager::SceneManager()
{
	// ������ ����� �� ���� ���� �ε�Ǵ� ��
	activeScene = LoadGameScene(0);
}

SceneManager::~SceneManager()
{

}

std::shared_ptr<Scene> SceneManager::LoadTitleScene()
{
	std::shared_ptr<TitleScene> scene = std::make_shared<TitleScene>();
	return scene;
}

std::shared_ptr<Scene> SceneManager::LoadGameScene(uint32_t p_id)
{
	std::shared_ptr<GameScene> scene = std::make_shared<GameScene>(p_id);
	return scene;
}