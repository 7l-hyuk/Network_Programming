#include "..\..\header\Common.h"

char* SERVERIP = (char*)"127.0.0.1";
#define SERVERPORT 9000
#define BUFSIZE    50


int main(int argc, char *argv[])
{
	int retval;

	if (argc > 1) SERVERIP = argv[1];

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 1;

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	inet_pton(AF_INET, SERVERIP, &serveraddr.sin_addr);
	serveraddr.sin_port = htons(SERVERPORT);

	retval = connect(
		sock,
		(struct sockaddr*)&serveraddr,
		sizeof(serveraddr)
	);
	if (retval == SOCKET_ERROR) err_quit("connect()");

	char buf[BUFSIZE];
    const char *testdata[] = {
        "안녕하세요",
        "반가워요",
        "오늘따라 할 얘기가 많을 거 같네요",
        "저도 그렇네요",
    };
	int len;

	for (int i = 0; i < 4; i++)
    {
        len = (int)strlen(testdata[i]);
		strncpy(buf, testdata[i], len);
		buf[len++] = '\n';

		retval = send(sock, buf, len, 0);
        if (retval ==  SOCKET_ERROR)
        {
            err_display("send()");
            break;
        }
        printf("[TCP Client] %d byte를 보냈습니다.\n", retval);
    }

	closesocket(sock);
	WSACleanup();
	return 0;
}