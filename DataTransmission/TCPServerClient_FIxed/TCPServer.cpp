#include "..\header\Common.h"

#define SERVERPORT 9000
#define BUFSIZE    50


int main(int argc, char *argv[])
{
    int retval;

    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        return 1;
    }

    SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock == INVALID_SOCKET)
    {
        err_quit("socket()");
    }

    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVERPORT);

    retval = bind(
        listen_sock,
        (struct sockaddr*)&serveraddr,
        sizeof(serveraddr)
    );
    if (retval == SOCKET_ERROR)
    {
        err_quit("bind()");
    }

    retval = listen(listen_sock, SOMAXCONN);
    if (retval == SOCKET_ERROR)
    {
        err_quit("listen()");
    }

    SOCKET client_sock;
    struct sockaddr_in clientaddr;
    int addrlen;
    char buf[BUFSIZE + 1];

    while (1)
    {
        addrlen = sizeof(clientaddr);
        client_sock = accept(
            listen_sock,
            (struct sockaddr*)&clientaddr,
            &addrlen
        );
        if (client_sock == INVALID_SOCKET)
        {
            err_display("accept()");
            break;
        }

        char addr[INET_ADDRSTRLEN];
        inet_ntop(
            AF_INET,
            &clientaddr.sin_addr,
            addr,
            sizeof(addr)
        );
        printf(
            "\n[TCP Server] Client 접속: IP = %s, port = %d\n",
            addr,
            ntohs(clientaddr.sin_port)
        );

        while (1)
        {
            retval = recv(client_sock, buf, BUFSIZE, MSG_WAITALL);
            if (retval == SOCKET_ERROR) 
            {
                err_display("recv()");
                break;
            }
            else if (retval == 0)
            {
                break;
            }

            buf[retval] = '\0';
            printf("[TCP/%s:%d] %s\n", addr, ntohs(clientaddr.sin_port), buf);
        }
        closesocket(client_sock);
        printf("[TCP Server] Client 종료: IP = %s, port = %d\n", addr, ntohs(clientaddr.sin_port));
    }
    closesocket(listen_sock);
    WSACleanup();
    return 0;
}