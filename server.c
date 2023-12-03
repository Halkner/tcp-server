#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Winsock2.h>
#include <ws2tcpip.h>

#define BUFFER_SIZE 1024
#define IP_ADDRESS "127.0.0.1"
#define PORT_NUMBER 3300

int main()
{
    WSADATA wsaData;
    SOCKET serverSock, clientSock;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrSize;
    char buffer[BUFFER_SIZE];

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        perror("[-] WSAStartup failed");
        exit(EXIT_FAILURE);
    }

    serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSock == INVALID_SOCKET)
    {
        perror("[-] Socket error");
        exit(EXIT_FAILURE);
    }
    printf("[+] TCP server socket created.\n");

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT_NUMBER);
    serverAddr.sin_addr.s_addr = inet_addr(IP_ADDRESS);

    if (bind(serverSock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        perror("[-] Bind error");
        closesocket(serverSock);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    printf("[+] Bind to the port number: %d\n", PORT_NUMBER);

    listen(serverSock, 5);
    printf("Listening...\n");

    while (1)
    {
        addrSize = sizeof(clientAddr);
        clientSock = accept(serverSock, (struct sockaddr *)&clientAddr, &addrSize);
        if (clientSock == INVALID_SOCKET)
        {
            perror("[-] Accept error");
            closesocket(serverSock);
            WSACleanup();
            exit(EXIT_FAILURE);
        }
        printf("[+] Client connected.\n");

        memset(buffer, 0, sizeof(buffer));
        recv(clientSock, buffer, sizeof(buffer), 0);
        printf("Client: %s\n", buffer);

        strcpy(buffer, "HI, THIS IS SERVER. HAVE A NICE DAY!!!");
        printf("Server: %s\n", buffer);
        send(clientSock, buffer, sizeof(buffer), 0);

        closesocket(clientSock);
        printf("[+] Client disconnected.\n\n");
    }

    closesocket(serverSock);
    WSACleanup();

    return 0;
}
