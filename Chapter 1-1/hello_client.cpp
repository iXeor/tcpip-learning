#include<winsock2.h>
#include <Ws2tcpip.h>
#include<iostream>
#include<string>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main(int argc,char** argv)
{
	if(argc != 3)
    {
	    cout << "Usage :"<< argv[0] <<" <IP Address> <port>" << endl;
        return -1;
    }

    WORD sockVersion = MAKEWORD(2, 2);
    WSADATA data;
    if (WSAStartup(sockVersion, &data) != 0)
    {
        return 1;
    }

    while (true){
        SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (clientSocket == INVALID_SOCKET){
            cout << "Socket error" << endl;
            return 1;
        }
        sockaddr_in sock_in;
        sock_in.sin_family = AF_INET;
        sock_in.sin_port = htons(atoi(argv[2]));
    	inet_pton(AF_INET,argv[1],&sock_in.sin_addr.S_un.S_addr);
        if (connect(clientSocket, (sockaddr*)&sock_in, sizeof(sock_in) )== SOCKET_ERROR){
            cout << "Connect error" << endl;
            return 1;
        }

        string data;
        getline(cin, data);
        const char * msg;
        msg = data.c_str();
        int msglen = strlen(msg);
        send(clientSocket, msg,msglen , 0);

        char recvdata[100];
        int num = recv(clientSocket, recvdata, 100, 0);
        if (num > 0){
            recvdata[num] = '\0';
            cout <<"Sever:"<< recvdata << endl;
        }
        closesocket(clientSocket);

    }
    WSACleanup();
    return 0;
}