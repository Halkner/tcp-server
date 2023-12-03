#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Winsock2.h>
#include <ws2tcpip.h>

int main()
{
    char *ip = "127.0.0.1";
    int port = 3300;

    int sock;
    struct sockaddr_in addr;
    socklen_t addr_size;
    char buffer[1024];
    int n;

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        perror("[-]WSAStartup failed");
        exit(1);
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET)
    {
        perror("[-]Socket error");
        WSACleanup();
        exit(1);
    }
    printf("[+] TCP client socket created.\n");

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port); // Convert port to network byte order
    addr.sin_addr.s_addr = inet_addr(ip);

    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR)
    {
        perror("[-]Connect error");
        closesocket(sock);
        WSACleanup();
        exit(1);
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
    printf("Disconnected from the server. \n");

    WSACleanup();

    return 0;
}
