#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <Winsock2.h>
#include <ws2tcpip.h>

int main()
{

    char *ip = "127.0.0.1";
    int port = 3300;

    SOCKET server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    char buffer[1024];
    int n;

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        perror("[-]WSAStartup failed");
        exit(1);
    }

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == INVALID_SOCKET)
    {
        perror("[-]Socket error");
        exit(1);
    }
    printf("[+] TCP server socket created.\n");

    memset(&server_addr, '\0', sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port); // Convert port to network byte order
    server_addr.sin_addr.s_addr = inet_addr(ip);

    n = bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (n == SOCKET_ERROR)
    {
        perror("[-]Bind error");
        closesocket(server_sock);
        WSACleanup();
        exit(1);
    }
    printf("[+] Bind to the port number: %d\n", port);

    listen(server_sock, 5);
    printf("Listening...\n");

    while (1)
    {
        addr_size = sizeof(client_addr);
        client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &addr_size);
        if (client_sock == INVALID_SOCKET)
        {
            perror("[-]Accept error");
            closesocket(server_sock);
            WSACleanup();
            exit(1);
        }
        printf("[+] Client connected. \n");

        memset(buffer, 0, sizeof(buffer));

        recv(client_sock, buffer, sizeof(buffer), 0);
        printf("Client: %s\n", buffer);

        memset(buffer, 0, sizeof(buffer));

        strcpy(buffer, "HI, THIS IS SERVER. HAVE A NICE DAY!!!");
        printf("Server: %s\n", buffer);
        send(client_sock, buffer, sizeof(buffer), 0);

        closesocket(client_sock); // Close the client socket when done
        printf("[+] Client disconnected.\n\n");
    }

    closesocket(server_sock);
    WSACleanup();

    return 0;
}
