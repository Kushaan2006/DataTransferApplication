#include<iostream>
#include<WinSock2.h>
#include <WS2tcpip.h> 
#include <fstream>

#pragma comment(lib, "ws2_32.lib")

using namespace std;


void recieve()
{
	system("cls");

	WSADATA wsaData;
	int res = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if ( res != 0)
	{
		cout << "ERROR: Failed to network! " << endl;
		return;
	}

	char host[256];
	gethostname(host, sizeof(host));

	cout << "Hi! " << host << " :D" << endl;
	cout << "Give this to your sender:\n";

	addrinfo hints{}, * info = nullptr;
	hints.ai_family = AF_INET;  

	getaddrinfo(host, nullptr, &hints, &info);

	sockaddr_in* ipv4 = (sockaddr_in*)info->ai_addr;
	char ipStr[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &ipv4->sin_addr, ipStr, sizeof(ipStr));

	cout << "IP: " << ipStr << endl;
	cout << "PORT: 8080\n\n";

	freeaddrinfo(info);


	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (sock == INVALID_SOCKET)
	{
		cout << "ERROR: Failed to create Scoket! " << endl;
		WSACleanup();
		return;
	}

	sockaddr_in addr{};

	addr.sin_family = AF_INET;
	addr.sin_port = htons(8080);
	addr.sin_addr.s_addr = INADDR_ANY;
	/*inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);*/

	res = bind(sock, (sockaddr*)&addr, sizeof(addr));

	if (res == SOCKET_ERROR)
	{
		cout << "ERROR: Failed to bind socket! " << WSAGetLastError() << endl;
		closesocket(sock);
		WSACleanup();
		return;
	}

	res = listen(sock, 5);

	if (res == SOCKET_ERROR)
	{
		cout << "ERROR: Failed to listen() ! " << endl;
		closesocket(sock);
		WSACleanup();
		return;
	}

	sockaddr_in sen_addr{};

	SOCKET clientSock = accept(sock, nullptr, nullptr);

	if (clientSock == INVALID_SOCKET)
	{
		cout << "ERROR: Failed to accept! " << WSAGetLastError() << endl;
		closesocket(sock);
		WSACleanup();
		return;
	}

	std::ofstream out("received.bin", std::ios::binary);

	if (!out)
	{
		cout << "ERROR: File Creation Failed!! " << endl;
		closesocket(clientSock);
		closesocket(sock);
		WSACleanup();
		return;
	}

	char buffer[4096];
	uint64_t totalRecv = 0;

	while (true)
	{
		int n = recv(clientSock, buffer, sizeof(buffer), 0);

		if (n > 0)
		{
			out.write(buffer, n);
			totalRecv += (uint64_t)n;

			system("cls");
			cout << "Received: " << totalRecv << " bytes" << endl;
		}

		else if (n == 0)
		{
			cout << "TRANSFER COMPLETE!";
			break;
		}

		else
		{
			cout << "ERROR: Transfer Failed! " << WSAGetLastError() << endl;
			break;
		}

	}

	out.close();

	closesocket(clientSock);
	closesocket(sock);
	WSACleanup();

}