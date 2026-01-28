#include<iostream>
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<string>
#include<fstream>

#pragma comment(lib, "ws2_32.lib")

using namespace std;


void send(const int port, const std::string& ip, const std::string& path)
{
	WSADATA wsaData;
	int res = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (res != 0)
	{
		cout << "ERROR: Failed to network! " << endl;
		return;
	}

	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (sock == INVALID_SOCKET)
	{
		cout << "ERROR: Failed to create Scoket! " << WSAGetLastError() << endl;
		WSACleanup();
		return;
	}

	sockaddr_in addr{};

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);

	if (inet_pton(AF_INET, ip.c_str(), &addr.sin_addr) != 1) 
	{
		cout << "ERROR: Invalid IP format!\n";
		closesocket(sock);
		WSACleanup();
		return;
	}


	res = connect(sock, (sockaddr*)&addr, sizeof(addr));

	if (res == SOCKET_ERROR)
	{
		cout << "ERROR: Couldnt connect! " << WSAGetLastError() << endl;
		closesocket(sock);
		WSACleanup();
		return;
	}

	std::ifstream in(path, std::ios::binary);

	if (!in)
	{
		cout << "ERROR: Failed to Locate File! " << endl;
		return;
	}

	in.seekg(0, std::ios::end);
	std::uint64_t fileSize = static_cast<std::uint64_t>(in.tellg());
	in.seekg(0, std::ios::beg);


	char buffer[4096];

	uint64_t totalSent = 0;

	while (in)
	{
		in.read(buffer, sizeof(buffer));
		std::streamsize n = in.gcount();
		
		if (n <= 0)
			break;

		int sent = ::send(sock, buffer, (int)n, 0);

		if (sent == SOCKET_ERROR) 
		{
			cout << "ERROR: send failed! " << WSAGetLastError() << endl;
			break;
		}

		system("cls");
		
		totalSent += (uint64_t)n;
		cout << "Progress: " << (((double)totalSent / fileSize) * 100) << "%" << endl;

	}

	shutdown(sock, SD_SEND);
	closesocket(sock);
	WSACleanup();

}
