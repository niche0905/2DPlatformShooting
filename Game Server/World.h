#pragma once
#include "pch.h"
#include <vector>
#include "CorePch.h"


class Object
{
	int temp;
};

class World
{
private:
	// vector�� �����Ѱ�? list���� ��ȯ ���ɼ�
	std::vector<Object> objects;	// ���忡 �ִ� ��ü�� (��� �� �÷��̾�, �Ѿ�, ������ ���)

	// ���⿡ ���� �浹 ó���� ���� �ٸ� �ڷᱸ������ �ʿ��� ���δ�?
	// Player 1, 2
	// Bullets
	// Items

public:
	World();

	void Update();	// ���ڷ� deltaTime�� �Ѱܾ� �� ������ ���δ�
	void Process();	// ��Ŷ�� ó���ϴ� �κ��� �� �� ����
	void Recv();	// recv �����忡�� ���� ��Ŷ���� ���� �޴� �κ��� �� ������ ���δ�

};

