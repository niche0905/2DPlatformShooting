#pragma once
#include "pch.h"
#include <vector>
#include "CorePch.h"
#include "Object.h"
#include "Player.h"
#include "Item.h"
#include "list"
#include "Level.h"
#include "Platform.h"


class Object;
class Level;

class World
{
public:
	// vector�� �����Ѱ�? list���� ��ȯ ���ɼ�
	//std::vector<Object> objects;	// ���忡 �ִ� ��ü�� (��� �� �÷��̾�, �Ѿ�, ������ ���)

	// ���⿡ ���� �浹 ó���� ���� �ٸ� �ڷᱸ������ �ʿ��� ���δ�?
	// Player 1, 2
	// Bullets
	// Items
	// �۽�ȣ �ڸ�Ʈ : �̷��� ������ Bullec ���Ի��� Item ���Ի��� �Ϸ��� �˻��� �ʿ��ѵ�
	//				 ��ü���� �������� �����̳ʰ� �ʿ��Ѱ� �ƴѰ�?
	//				 objects �� ���� ������ �𸣰ڴ�

	TimerManager* tm;		// TimerManager

	// TODO : array�� ���� �� �� �ְ�
	//		  Bullet�� Player �ȿ� ���Խ�Ű��
	Player p1;
	Player p2;

	// �۽�ȣ 11/27 Player ���η� ����
	//std::list<Bullet> b1;	// p1�� �� �Ѿ�
	//std::list<Bullet> b2;	// p2�� �� �Ѿ�

	std::list<Item> items;	// item�� ������ ��� �ִ� �ڷᱸ��

	Level level;	// �÷����� (����)

public:
	World();

	void Update();	// ���ڷ� deltaTime�� �Ѱܾ� �� ������ ���δ�
	void CollisionCheck();	// World�� ��ҵ��� �浹ó�� ��
	void Process();	// ��Ŷ�� ó���ϴ� �κ��� �� �� ����
	void Recv();	// recv �����忡�� ���� ��Ŷ���� ���� �޴� �κ��� �� ������ ���δ�

};

