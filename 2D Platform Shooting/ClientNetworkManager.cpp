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
    // 윈속 초기화
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return;

    // 소켓 생성
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) //err_quit("socket()");

    // 이벤트 핸들 생성
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

    // 큐 초기화
    while (!process_queue.empty()) {
        process_queue.pop();
    }

    // 클라이언트 ID 초기화
    ClientID = -1;

    // 스레드 핸들 초기화
    clientThread = NULL;
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