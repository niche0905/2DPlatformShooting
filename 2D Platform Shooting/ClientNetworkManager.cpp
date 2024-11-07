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
    process_queue = std::queue<std::array<char, MAX_SIZE>>();

    // Ŭ���̾�Ʈ ID �ʱ�ȭ
    ClientID = -1;

    // ������ �ڵ� �ʱ�ȭ
    clientThread = NULL;
}

void ClientNetworkManager::Connect()
{
    // ���� �ּ� ����
    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(SERVER_PORT);

    // inet_pton ����Ͽ� IP �ּ� ��ȯ
    if (inet_pton(AF_INET, "127.0.0.1", &serveraddr.sin_addr) != 1) {
        closesocket(clientSocket);
        WSACleanup();
        return;
    }

    // ���� �õ�
    int result = connect(clientSocket, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
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
    // ���ο� ���� �迭 ����
    std::array<char, MAX_SIZE> newBuffer;

    // ���� ������ �����͸� �� �迭�� ����
    memcpy(newBuffer.data(), buf, MAX_SIZE);

    // ť�� ���� �߰�
    process_queue.push(newBuffer);

    // ó�� �̺�Ʈ ����
    SetEvent(processEvent);
}

void ClientNetworkManager::SendPacket(char buf[MAX_SIZE])
{

}

void ClientNetworkManager::Update()
{

}