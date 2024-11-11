#include "pch.h"
#include "ServerNetworkManager.h"

ServerNetworkManager::ServerNetworkManager()
{
}

void ServerNetworkManager::Init()
{
}

void ServerNetworkManager::CreateLobbyThread()
{
}

void ServerNetworkManager::CreateUpdateThread()
{
}

void ServerNetworkManager::CreateRecvThread(HANDLE socket)
{
}

void ServerNetworkManager::PushBuffer(BufferType buffer)
{
}

QueueType& ServerNetworkManager::GetQueue()
{
	return processQueue[0];
}

void ServerNetworkManager::SendPacket(PacketType packet)
{
}

DWORD WINAPI workerUpdate(LPVOID arg)
{
	return 0;
}

DWORD WINAPI workerRecv(LPVOID arg)
{
	return 0;
}

DWORD WINAPI workerLobby(LPVOID arg)
{
	return 0;
}
