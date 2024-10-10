#include "..\header\Common.h"


int main(int argc, char* argv[])
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 1;
	printf("[알림] WinSock Initialized!\n");

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");
	printf("[알림] socket 생성 성공\n");

	closesocket(sock);

	WSACleanup();
	return 0;
}