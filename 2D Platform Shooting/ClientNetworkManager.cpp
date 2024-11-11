#include "pch.h"
#include "ClientNetworkManager.h"

DWORD WINAPI WorkerRecv(LPVOID arg)
{
    ClientNetworkManager* network = (ClientNetworkManager*)arg;
    WSAEVENT events[1] = { network->GetRecvEvent()};
    char buf[MAX_SIZE];

    while (true) {
        // 이벤트 대기
        DWORD result = WSAWaitForMultipleEvents(1, events, FALSE, WSA_INFINITE, FALSE);
        if (result == WSA_WAIT_FAILED) break;

        // 이벤트 확인
        WSANETWORKEVENTS networkEvents;
        if (WSAEnumNetworkEvents(network->GetSocket(), network->GetRecvEvent(), &networkEvents) == SOCKET_ERROR)
            break;

        // 연결 종료 확인
        if (networkEvents.lNetworkEvents & FD_CLOSE) {
            break;
        }

        // 데이터 수신
        if (networkEvents.lNetworkEvents & FD_READ) {
            int recvLen = recv(network->GetSocket(), buf, MAX_SIZE, 0);
            if (recvLen > 0) {
                network->PushBuffer(buf);
            }
            else if (recvLen == 0 || recvLen == SOCKET_ERROR) {
                break;
            }
        }
    }
    return 0;
}

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
    process_queue = std::queue<std::array<char, MAX_SIZE>>();

    // 클라이언트 ID 초기화
    ClientID = -1;

    // 스레드 핸들 초기화
    clientThread = NULL;
}

void ClientNetworkManager::Connect()
{
    // 서버 주소 설정
    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(SERVER_PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serveraddr.sin_addr) != 1) {
        closesocket(clientSocket);
        WSACleanup();
        return;
    }

    // 연결 시도
    int result = connect(clientSocket, (struct sockaddr*)&serveraddr, sizeof(serveraddr));
    if (result == SOCKET_ERROR) {
        closesocket(clientSocket);
        WSACleanup();
        return;
    }

    result = WSAEventSelect(clientSocket, recvEvent, FD_READ | FD_CLOSE);
    if (result == SOCKET_ERROR) {
        closesocket(clientSocket);
        WSACleanup();
        return;
    }

    CreateRecvThread();
}

void ClientNetworkManager::CreateRecvThread()
{
    // 이미 스레드가 존재하는 경우 처리
    if (clientThread != NULL)
    {
        CloseHandle(clientThread);
        clientThread = NULL;
    }

    // WorkerRecv 스레드 생성
    clientThread = CreateThread(NULL, 0, WorkerRecv, (LPVOID)this, 0, NULL);

    // 스레드 생성 실패 시 처리
    if (clientThread == NULL)
    {
        // 소켓 닫기
        if (clientSocket != INVALID_SOCKET)
        {
            closesocket(clientSocket);
            clientSocket = INVALID_SOCKET;
        }

        // 이벤트 핸들 닫기
        if (recvEvent != NULL)
        {
            CloseHandle(recvEvent);
            recvEvent = NULL;
        }

        if (processEvent != NULL)
        {
            CloseHandle(processEvent);
            processEvent = NULL;
        }

        // 큐 비우기
        while (!process_queue.empty())
        {
            process_queue.pop();
        }

        // 클라이언트 ID 초기화
        ClientID = -1;

        WSACleanup();
    }
}

void ClientNetworkManager::PushBuffer(char buf[MAX_SIZE])
{
    // 새로운 버퍼 배열 생성
    std::array<char, MAX_SIZE> newBuffer;

    // 받은 버퍼의 데이터를 새 배열에 복사
    memcpy(newBuffer.data(), buf, MAX_SIZE);

    // 큐에 버퍼 추가
    process_queue.push(newBuffer);

    // 처리 이벤트 설정
    SetEvent(processEvent);
}

void ClientNetworkManager::SendPacket(char buf[MAX_SIZE])
{

}

void ClientNetworkManager::Update()
{

}