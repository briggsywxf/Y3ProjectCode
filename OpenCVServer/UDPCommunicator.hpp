//
// Created by sbrig on 11/02/2023.
//

#pragma once

#include <WinSock2.h>

#define BUFLEN 512
#define SERVERPORT 25555
#define GAMEPORT 25554
//#define GAMEPORT 53054

class UDPCommunicator {
public:
    UDPCommunicator();

    bool SendTrackingData(std::pair<float, float> trackingData);

    void WaitForStartRequest();

private:
    SOCKET s;
    struct sockaddr_in serverAddr, gameAddr;
    int gameAddrLen, recv_len;
    char receiveBuf[BUFLEN];
    char* sendBuf;
    int sendBufSize = sizeof(char) + 2 * sizeof(float);
    WSADATA wsa;

};
