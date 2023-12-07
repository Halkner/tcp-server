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

    int sock;
    struct sockaddr_in addr;
    char buffer[BUFFER_SIZE];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("[-] Socket error");
        #ifdef _WIN32
        WSACleanup();
        #endif
        exit(EXIT_FAILURE);
    }
    printf("[+] TCP client socket created.\n");

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT_NUMBER);
    #ifdef _WIN32
    addr.sin_addr.s_addr = inet_addr(IP_ADDRESS);
    #else
    inet_pton(AF_INET, IP_ADDRESS, &(addr.sin_addr));
    #endif

    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("[-] Connect error");
        #ifdef _WIN32
        closesocket(sock);
        WSACleanup();
        #else
        close(sock);
        #endif
        exit(EXIT_FAILURE);
    }
    printf("Connected to the server.\n");

    memset(buffer, 0, sizeof(buffer));

    strcpy(buffer, "HELLO, THIS IS CLIENT.");
    printf("Client: %s\n", buffer);
    send(sock, buffer, strlen(buffer), 0);

    memset(buffer, 0, sizeof(buffer));

    recv(sock, buffer, sizeof(buffer), 0);
    printf("Server: %s\n", buffer);

    #ifdef _WIN32
    closesocket(sock);
    WSACleanup();
    #else
    close(sock);
    #endif
    printf("Disconnected from the server.\n");

    return 0;
}
