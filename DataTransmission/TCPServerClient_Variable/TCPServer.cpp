#include "..\..\header\Common.h"

#define SERVERPORT 9000
#define BUFSIZE    512


int _recv_ahead(SOCKET s, char *p)
{
    __declspec(thread) static int nbytes = 0;
    __declspec(thread) static char buf[1024];
    __declspec(thread) static char *ptr;

    if (nbytes == 0 || nbytes == SOCKET_ERROR)
    {
        nbytes = recv(s, buf, sizeof(buf), 0);
        if (nbytes == SOCKET_ERROR) return SOCKET_ERROR;
        else if (nbytes == 0) return 0;
        ptr = buf;
    }

    --nbytes;
    *p = *ptr++;

    return 1;
}


int recvline(SOCKET s, char *buf, int maxlen)
{
    int n, nbytes;
    char c, *ptr = buf;

    for (n = 1; n < maxlen; n++)
    {
        nbytes = _recv_ahead(s, &c);
        if (nbytes == 1)
        {
            *ptr++ = c;
            if (c == '\n') break;
        }
        else if (nbytes == 0)
        {
            *ptr = 0;
            return n - 1;
        }
        else
        {
            return SOCKET_ERROR;
        }
    }

    *ptr = 0;
    return n;
}


int main(int argc, char *argv[])
{
    int retval;

    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 1;

    // 대기 소켓용 소켓 초기화 (listen socket)
    SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock == INVALID_SOCKET) err_quit("socket()");

    //bind()
    struct sockaddr_in serveraddr;
    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htons(INADDR_ANY);
    serveraddr.sin_port = htons(SERVERPORT);

    retval = bind(
        listen_sock,
        (struct sockaddr*)&serveraddr,
        sizeof(serveraddr)
    );
    if (retval == SOCKET_ERROR) err_quit("bind()");

    // listen()
    retval = listen(listen_sock , SOMAXCONN);
    if (retval == SOCKET_ERROR) err_quit("listen()");

    // client socket
    SOCKET client_sock;
    struct sockaddr_in clientaddr;
    int addrlen;
    char buf[BUFSIZE + 1];

    while (1)
    {
        // accept(): 접속한 Client Socket 생성
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

        // Client 정보
        char addr[INET_ADDRSTRLEN];
        inet_ntop(
            AF_INET,
            &clientaddr.sin_addr,
            addr,
            sizeof(addr)
        );
        printf(
            "\n[TCP Server] Client 접속: IP = %s, Port = %d\n",
            addr,
            ntohs(clientaddr.sin_port)
        );

        while (1)
        {
            // recv(): Client 데이터 수신
            retval = recvline(client_sock, buf, BUFSIZE + 1);
            if (retval == SOCKET_ERROR)
            {
                err_display("recv()");
                break;
            }
            else if (retval == 0)
            {
                break;
            }

            printf(
                "[TCP/%s:%d] %s",
                addr,
                ntohs(clientaddr.sin_port),
                buf
            );
        }
        closesocket(client_sock);
        printf(
            "[TCP Server] Client 종료: IP = %s, Port = %d\n",
            addr,
            ntohs(clientaddr.sin_port)
        );
    }
    closesocket(listen_sock);
    WSACleanup();
    return 0;
}