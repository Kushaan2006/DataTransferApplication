#include<iostream>
#include<string>

using namespace std;

void send(const int, const string&, const string&);
void recieve();

int main()
{
	int choice;
	cout << "Welcome! File TRANSFER!!!" << endl;
	cout << "1.) Send" << endl;
	cout << "2.) Recieve" << endl;
	cout << "Your Choice: ";
	cin >> choice;

	string ip{};
	string path{};
	int port;

	switch (choice)
	{
	case 1:
		
		cout << endl << "Enter IP: ";
		cin >> ip;
		cout << "Enter PORT: ";
		cin >> port;
		cout << "Enter File Path: ";
		cin >> path;

		send(port, ip, path);
		break;

	case 2:
		recieve();
		break;
	default:
		cout << "Please enter a valid choice" << endl;
	}

	getchar();
}