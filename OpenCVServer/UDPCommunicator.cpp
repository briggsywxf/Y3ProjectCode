//
// Created by sbrig on 11/02/2023.
//

#include <iostream>
#include "UDPCommunicator.hpp"

#pragma comment(lib, "ws2_32.lib")

UDPCommunicator::UDPCommunicator()
{
    gameAddrLen = sizeof(gameAddr);

    sendBuf = new char[sendBufSize];

    // Initialise winsock
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("Failed. Error code: %d", WSAGetLastError());
        throw;
    }

    // Create a socket
    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket: %d", WSAGetLastError());
        throw;
    }

    unsigned long ul = 1;
    int err = ioctlsocket(s, FIONBIO, (unsigned long *) &ul);
    if (err == SOCKET_ERROR) {
        std::cout << "Failed to put socket in non-blocking mode";
        throw;
    }
    printf("Socket created.\n");

    // Prepare the sockaddr_in structure
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(SERVERPORT);

    gameAddr.sin_family = AF_INET;
    gameAddr.sin_addr.s_addr = inet_addr("192.168.0.68");
    gameAddr.sin_port = htons(GAMEPORT);

    // Bind
    if (bind(s, (sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        printf("Bind failed with error code: %d", WSAGetLastError());
        throw;
    }
}


bool UDPCommunicator::SendTrackingData(std::pair<float, float> trackingData) {
    memset(sendBuf, 0, sendBufSize);

    // Packet type "track data"
    sendBuf[0] = 3;
    // X float
    memcpy(&sendBuf[1], &trackingData.first, sizeof(float));
    // Y float
    memcpy(&sendBuf[1 + sizeof(float)], &trackingData.second, sizeof(float));

    int err = sendto(s, sendBuf, sizeof(char) + 2 * sizeof(float), 0, (sockaddr*) &gameAddr, sizeof(gameAddr));


    if (err == SOCKET_ERROR) {
        std::cout << "Send error: %d " << WSAGetLastError() << std::endl;
        return false;
    }

    return true;
}

void UDPCommunicator::WaitForStartRequest() {
    // TODO: Timeout
    while (true) {
        int recvErr = recvfrom(s, receiveBuf, BUFLEN, 0, (sockaddr*)&gameAddr, &gameAddrLen);

        if (recvErr == SOCKET_ERROR) {
            // If not just an empty buffer
            if (WSAGetLastError() != WSAEWOULDBLOCK) {
                std::cout << "WSA Error: %d " << WSAGetLastError() << std::endl;
                throw;
            }
        } else {
            char requestType = 0;
            // Copy type field
            memcpy(&requestType, receiveBuf, sizeof(char));

            // If request to start tracking
            if (requestType == 1) {
                std::cout << "Tracking starting\n";
                return;
            }
        }
    }

}
