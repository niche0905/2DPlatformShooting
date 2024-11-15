#include "pch.h"
#include "SceneManager.h"

SceneManager::SceneManager()
{
	// ������ ����� �� ���� ���� �ε�Ǵ� ��
	activeScene = LoadTitleScene();		// TEMP : �׽�Ʈ�� ���� (11/15 �۽�ȣ)
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