#include "..\header\Common.h"


int main(int argc, char *argv[])
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 1;


	/*---------------IPv4 Test---------------*/
	const char* ipv4test = "147.46.114.70";
	printf("NOT CONVERTED: IPv4 addr = %s\n", ipv4test);

	// inet_pton
	struct in_addr ipv4num;
	inet_pton(AF_INET, ipv4test, &ipv4num);
	printf("CONVERTED: IPv4 addr = %#x\n", ipv4num.s_addr);

	// inet_ntop
	char ipv4str[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &ipv4num, ipv4str, sizeof(ipv4str));
	printf("CONVERTED AGAIN: IPv4 addr = %s\n", ipv4str);
	printf("\n");


	/*---------------IPv6 Test---------------*/
	const char* ipv6test = "2001:0230:abcd:ffab:0023:eb00:ffff:1111";
	printf("NOT CONVERTED: IPv6 addr = %s\n", ipv6test);

	// inet_pton
	struct in6_addr ipv6num;
	inet_pton(AF_INET6, ipv6test, &ipv6num);
	printf("CONVERTED: IPv6 addr = 0x");
	for (int i = 0; i < 16; i++) printf("%02x", ipv6num.s6_addr[i]);
	printf("\n");

	// inet_ntop
	char ipv6str[INET6_ADDRSTRLEN];
	inet_ntop(AF_INET6, &ipv6num, ipv6str, sizeof(ipv6str));
	printf("CONVERTED AGAIN: IPv6 addr = %s\n", ipv6str);
	printf("\n");

	WSACleanup();
	return 0;
}

