/* Written by Can Alper
 *
 * Env server code.
 *
 */

#define WIN32_LEAN_AND_MEAN

#include <iostream>

#include "HiveEngine/Entity.h"
#include "HiveEngine/Utilities.h"
#include "CopterLib/Environment.h"

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#include <iostream>
#include <thread>

#include "CopterLib/Environment.h"
#include <vector>

#define DEFAULT_PORT "30000"

void serve(SOCKET ClientSocket);

int __cdecl main(int argc, char** argv){
    std::cout << "-- START --" << std::endl;

    WSADATA wsaData;

    auto c = CopterLib::Copter({1, 3, 2}, 32, 45); // if you disable this line it does not compile wtf...
    //CopterLib::Environment env;
    //env.command(c.serialize());

    int iResult;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }

    struct addrinfo *result = NULL, *ptr = NULL, hints;

    ZeroMemory(&hints, sizeof (hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the local address and port to be used by the server
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    SOCKET ListenSocket = INVALID_SOCKET;
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    if (ListenSocket == INVALID_SOCKET) {
        printf("Error at socket(): %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Setup the TCP listening socket
    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    if ( listen( ListenSocket, SOMAXCONN ) == SOCKET_ERROR ) {
        printf( "Listen failed with error: %ld\n", WSAGetLastError() );
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    SOCKET ClientSocket;

    ClientSocket = INVALID_SOCKET;


    while (true) {
        // Accept a client socket
        ClientSocket = accept(ListenSocket, NULL, NULL);

        if (ClientSocket == INVALID_SOCKET) {
            printf("accept failed: %d\n", WSAGetLastError());
            closesocket(ListenSocket);
            WSACleanup();
        } else {
            std::thread session(&serve, ClientSocket);
            session.detach();
        }
    }

    closesocket(ListenSocket);
    WSACleanup();

    std::cout << "--  END  --" << std::endl;
    return 0;
}



void serve(SOCKET ClientSocket){
    // Receive until the peer shuts down the connection
    CopterLib::Environment env;
    do {
        char header_buf[sizeof(unsigned)];
        int iResult, iSendResult;

        iResult = recv(ClientSocket, header_buf, sizeof(unsigned), 0);
        if (iResult == sizeof(unsigned)) {
            unsigned size = *(unsigned*)header_buf;
            char data_buf[size];
            iResult = recv(ClientSocket, data_buf, size, 0);
            if(iResult == size){

                std::vector<char> vec_form(data_buf, data_buf+size);
                auto res = env.command(vec_form);
                // Echo the result back to the sender
                auto reply_size = (unsigned)res.size();

                iSendResult = send(ClientSocket, (char*)&reply_size, sizeof(unsigned), 0);
                if (iSendResult == SOCKET_ERROR) {
                    closesocket(ClientSocket);
                    break;
                }

                iSendResult = send(ClientSocket, res.data(), (unsigned)res.size(), 0);
                if (iSendResult == SOCKET_ERROR) {
                    closesocket(ClientSocket);
                    break;
                }

            } else {
                closesocket(ClientSocket);
                break;
            }

        } else {
            closesocket(ClientSocket);
            break;
        }

    } while (true);
    printf("Finishing thread\n");
}