#include "pch.h"
#include "ClientNetworkManager.h"

ClientNetworkManager::ClientNetworkManager()
{

}

ClientNetworkManager::~ClientNetworkManager()
{

}

void ClientNetworkManager::Init()
{
    // ���� �ʱ�ȭ
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return;

    // ���� ����
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == INVALID_SOCKET) //err_quit("socket()");

    // �̺�Ʈ �ڵ� ����
    recvEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (recvEvent == NULL) {
        closesocket(client_socket);
        //err_quit("Failed to create receive event");
    }

    processEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (processEvent == NULL) {
        CloseHandle(recvEvent);
        closesocket(client_socket);
        //err_quit("Failed to create process event");
    }

    // ť �ʱ�ȭ
    while (!process_queue.empty()) {
        process_queue.pop();
    }

    // Ŭ���̾�Ʈ ID �ʱ�ȭ
    ClientID = -1;

    // ������ �ڵ� �ʱ�ȭ
    client_thread = NULL;
}

void ClientNetworkManager::Connect()
{

}

void ClientNetworkManager::CreateRecvThread()
{

}

void ClientNetworkManager::PushBuffer(char buf[MAX_SIZE])
{

}

void ClientNetworkManager::SendPacket(char buf[MAX_SIZE])
{

}

void ClientNetworkManager::PushBuffer()
{

}

void ClientNetworkManager::Update()
{

}

void ClientNetworkManager::Draw()
{

}