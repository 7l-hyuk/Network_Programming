#include "..\header\Common.h"

char* SERVERIP = (char*)"127.0.0.1";
#define SERVERPORT 9000
#define BUFSIZE    512


int main(int argc, char* argv[])
{
	int retval;

	if (argc > 1) SERVERIP = argv[1];

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 1;

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// 포트 번호를 50000으로 고정한다.
	struct sockaddr_in localaddr;
	memset(&localaddr, 0, sizeof(localaddr));

	localaddr.sin_family = AF_INET;
	localaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	localaddr.sin_port = htons(50000);

	retval = bind(sock, (struct sockaddr*)&localaddr, sizeof(localaddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");


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

	//char buf[BUFSIZE + 1];
	//int len;
	int num[2];
	int sum;

	while (1)
	{
		/*printf("\n[보낼 데이터] ");
		if (fgets(buf, BUFSIZE + 1, stdin) == NULL) break;

		len = (int)strlen(buf);
		if (buf[len - 1] == '\n') buf[len - 1] = '\0';
		if (strlen(buf) == 0) break;*/
		printf("\n[정수 두 개 입력]");
		scanf("%d %d", &num[0], &num[1]);

		retval = send(sock, (const char*)num, sizeof(num), 0);
		if (retval == SOCKET_ERROR)
		{
			err_display("send()");
			break;
		}

		printf("[TCP Client] %d byte를 보냈습니다.\n", retval);

		//retval = recv(sock, buf, retval, MSG_WAITALL);
		retval = recv(sock, (char*)&sum, sizeof(sum), MSG_WAITALL);
		if (retval == SOCKET_ERROR)
		{
			err_display("recv()");
			break;
		}
		else if (retval == 0) break;

		//buf[retval] = '\0';
		printf("[TCP Client] %d byte를 받았습니다.\n", retval);
		//printf("[받은 데이터] %s\n", buf);
		printf("[받은 데이터] %d\n", sum);
	}

	closesocket(sock);

	WSACleanup();
	return 0;
}