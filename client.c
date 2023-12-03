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
    SOCKET sock;
    struct sockaddr_in addr;
    socklen_t addrSize;
    char buffer[BUFFER_SIZE];

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        perror("[-] WSAStartup failed");
        exit(EXIT_FAILURE);
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET)
    {
        perror("[-] Socket error");
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    printf("[+] TCP client socket created.\n");

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT_NUMBER);
    addr.sin_addr.s_addr = inet_addr(IP_ADDRESS);

    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR)
    {
        perror("[-] Connect error");
        closesocket(sock);
        WSACleanup();
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

    closesocket(sock);
    printf("Disconnected from the server.\n");

    WSACleanup();

    return 0;
}
