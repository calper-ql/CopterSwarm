#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>


// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#include "CopterLib/Environment.h"
#include "CopterLib/Serializable.h"

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "30000"

#include <string>

void send_serialized(SOCKET ConnectSocket, CopterLib::Serializable* s){
    int iResult;
    auto serial = s->serialize();
    auto pckt_size = (unsigned)serial.size();
    iResult = send( ConnectSocket, (char*)&pckt_size, sizeof(unsigned), 0 );
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return;
    }
    iResult = send( ConnectSocket, serial.data(), (unsigned)serial.size(), 0 );
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return;
    }

    std::vector<char> command;

    char header_buf[sizeof(unsigned)];
    iResult = recv(ConnectSocket, header_buf, sizeof(unsigned), 0);
    if (iResult == sizeof(unsigned)) {
        unsigned size = *(unsigned *) header_buf;
        char data_buf[size];
        iResult = recv(ConnectSocket, data_buf, size, 0);
        command = std::vector<char>(data_buf, data_buf+iResult);
    }

    std::cout <<  CopterLib::unpack_command(command) << " " << CopterLib::unpack_unsigned(command) << std::endl;
}

int __cdecl main(int argc, char **argv)
{

    auto c1 = CopterLib::Copter({1, 3, 2}, 32, 45); // must be called, again wtf...

    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo *result = NULL,
            *ptr = NULL,
            hints;
    char *sendbuf = "this is a test";
    char recvbuf[DEFAULT_BUFLEN];
    int iResult;
    int recvbuflen = DEFAULT_BUFLEN;

    char * server_address;

    // Validate the parameters
    if (argc < 2) {
        //printf("usage: %s server-name\n", argv[0]);
        //return 1;
        server_address = (char*)std::string("127.0.0.1").c_str();
    }

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo(server_address, DEFAULT_PORT, &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Attempt to connect to an address until one succeeds
    for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
                               ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }

        // Connect to server.
        iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }

    CopterLib::Copter* c = new CopterLib::Copter(glm::vec3(0.0, 0.0, 1.0), 1.0, 1.0123);

    CopterLib::EnergySource* es = new CopterLib::EnergySource(3600.0f*12.0f*3.6f, 3600.0f*12.0f*3.6f, glm::vec3(0.0, 0.0, 0.1), 0.1, 0.1);

    CopterLib::Motor* mlu = new CopterLib::Motor(300, 0.05, glm::vec3(-1.0, 1.0, 0.0), 0.1, 0.1);
    CopterLib::Motor* mll = new CopterLib::Motor(300, 0.05, glm::vec3(-1.0, -1.0, 0.0), 0.1, 0.1);
    CopterLib::Motor* mru = new CopterLib::Motor(300, 0.05, glm::vec3(1.0, 1.0, 0.0), 0.1, 0.1);
    CopterLib::Motor* mrl = new CopterLib::Motor(300, 0.05, glm::vec3(1.0, -1.0, 0.0), 0.1, 0.1);


    CopterLib::Rotor* rlu = new CopterLib::Rotor(4.0f, 0.09f * 1.225f * 1.0f, true, glm::vec3(0.0, 0.0, 0.1), 0.2, 0.1);
    CopterLib::Rotor* rll = new CopterLib::Rotor(4.0f, 0.09f * 1.225f * 1.0f, true, glm::vec3(0.0, 0.0, 0.1), 0.2, 0.1);
    CopterLib::Rotor* rru = new CopterLib::Rotor(4.0f, 0.09f * 1.225f * 1.0f, true, glm::vec3(0.0, 0.0, 0.1), 0.2, 0.1);
    CopterLib::Rotor* rrl = new CopterLib::Rotor(4.0f, 0.09f * 1.225f * 1.0f, true, glm::vec3(0.0, 0.0, 0.1), 0.2, 0.1);

    send_serialized(ConnectSocket, c);

    send_serialized(ConnectSocket, es);

    send_serialized(ConnectSocket, mlu);
    send_serialized(ConnectSocket, mll);
    send_serialized(ConnectSocket, mru);
    send_serialized(ConnectSocket, mrl);

    send_serialized(ConnectSocket, rlu);
    send_serialized(ConnectSocket, rll);
    send_serialized(ConnectSocket, rru);
    send_serialized(ConnectSocket, rrl);


    // cleanup
    closesocket(ConnectSocket);
    WSACleanup();

    return 0;
}