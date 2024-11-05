#define _WINSOCK_DEPRECATED_NO_WARNINGS
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
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) //err_quit("socket()");

    // �̺�Ʈ �ڵ� ����
    recvEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (recvEvent == NULL) {
        closesocket(clientSocket);
        //err_quit("Failed to create receive event");
    }

    processEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (processEvent == NULL) {
        CloseHandle(recvEvent);
        closesocket(clientSocket);
        //err_quit("Failed to create process event");
    }

    // ť �ʱ�ȭ
    while (!process_queue.empty()) {
        process_queue.pop();
    }

    // Ŭ���̾�Ʈ ID �ʱ�ȭ
    ClientID = -1;

    // ������ �ڵ� �ʱ�ȭ
    clientThread = NULL;
}

void ClientNetworkManager::Connect()
{
    // ���� �ּ� ����
    SOCKADDR_IN serverAddr;
    ZeroMemory(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // ���� �õ�
    int result = connect(clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
    if (result == SOCKET_ERROR) {
        closesocket(clientSocket);
        WSACleanup();
        return;
    }

    // WSAEventSelect ���� - �񵿱� �̺�Ʈ ó���� ����
    result = WSAEventSelect(clientSocket, recvEvent, FD_READ | FD_CLOSE);
    if (result == SOCKET_ERROR) {
        closesocket(clientSocket);
        WSACleanup();
        return;
    }

    // ���� ���� �� ���� ������ ����
    CreateRecvThread();
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