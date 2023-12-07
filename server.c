#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <Winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <unistd.h>
#include <arpa/inet.h>
#endif

#define BUFFER_SIZE 1024
#define IP_ADDRESS "127.0.0.1"
#define PORT_NUMBER 3300

int main()
{
    #ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        perror("[-] WSAStartup failed");
        exit(EXIT_FAILURE);
    }
    #endif

    int serverSock, clientSock;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrSize;
    char buffer[BUFFER_SIZE];

    serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSock < 0)
    {
        perror("[-] Socket error");
        #ifdef _WIN32
        WSACleanup();
        #endif
        exit(EXIT_FAILURE);
    }
    printf("[+] TCP server socket created.\n");

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT_NUMBER);
    #ifdef _WIN32
    serverAddr.sin_addr.s_addr = inet_addr(IP_ADDRESS);
    #else
    inet_pton(AF_INET, IP_ADDRESS, &(serverAddr.sin_addr));
    #endif

    if (bind(serverSock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        perror("[-] Bind error");
        #ifdef _WIN32
        closesocket(serverSock);
        WSACleanup();
        #else
        close(serverSock);
        #endif
        exit(EXIT_FAILURE);
    }
    printf("[+] Bind to the port number: %d\n", PORT_NUMBER);

    listen(serverSock, 5);
    printf("Listening...\n");

    while (1)
    {
        addrSize = sizeof(clientAddr);
        clientSock = accept(serverSock, (struct sockaddr *)&clientAddr, &addrSize);
        if (clientSock < 0)
        {
            perror("[-] Accept error");
            #ifdef _WIN32
            closesocket(serverSock);
            WSACleanup();
            #else
            close(serverSock);
            #endif
            exit(EXIT_FAILURE);
        }
        printf("[+] Client connected.\n");

        memset(buffer, 0, sizeof(buffer));
        recv(clientSock, buffer, sizeof(buffer), 0);
        printf("Client: %s\n", buffer);

        strcpy(buffer, "HI, THIS IS SERVER. HAVE A NICE DAY!!!");
        printf("Server: %s\n", buffer);
        send(clientSock, buffer, sizeof(buffer), 0);

        #ifdef _WIN32
        closesocket(clientSock);
        #else
        close(clientSock);
        #endif
        printf("[+] Client disconnected.\n\n");
    }

    #ifdef _WIN32
    closesocket(serverSock);
    WSACleanup();
    #else
    close(serverSock);
    #endif

    return 0;
}
