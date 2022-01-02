#include <winsock2.h>
#include <Ws2tcpip.h>
#include <iostream>
#include <string>

#pragma comment(lib,"ws2_32.lib")

using namespace std;

int main(int argc,char** argv)
{
    if(argc != 2)
    {
	    cout << "Usage :"<< argv[0] <<" <port>" << endl;
        return -1;
    }

    char msg[100];

	WORD sockVersion = MAKEWORD(2, 2);
    WSADATA wsdata;
    if (WSAStartup(sockVersion, &wsdata) != 0)
    {
        return 1;
    }

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET)
    {
        cout << "Socket error" << endl;
        return 1;
    }

    sockaddr_in sockAddr;
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port = htons(atoi(argv[1]));
    sockAddr.sin_addr.S_un.S_addr = INADDR_ANY;

    if (bind(serverSocket, (sockaddr*)&sockAddr, sizeof(sockAddr)) == SOCKET_ERROR){
        cout << "Bind error" << endl;
        return 1;
    }

    if(listen(serverSocket,10) == SOCKET_ERROR)
    {
	    cout << "Listen error" << endl;
        return 1;
    }

    SOCKET clientSocket;
    sockaddr_in client_sin;
    bool flag = false;
    int length = sizeof(client_sin);
	char addrStr[INET_ADDRSTRLEN];

    while (true){
        if (!flag)
            cout << "Connecting..." << endl;
        clientSocket = accept(serverSocket, (sockaddr*)&client_sin, &length);
        if (clientSocket == INVALID_SOCKET){
            cout << "Accept error" << endl;
            flag = false;
            return 1;
        }
        if (!flag)
            cout << "Get Connected @ " << inet_ntop(AF_INET,&client_sin.sin_addr,addrStr,sizeof(addrStr)) << endl;
        flag = true;
        int num = recv(clientSocket, msg, 100, 0);
        if (num > 0)
        {
            msg[num] = '\0';
            cout <<"Client: "<< msg << endl;

        }
        string data;
        getline(cin, data);
        const char * sendData;
        sendData = data.c_str();
        int msglen = strlen(sendData);
        send(clientSocket, sendData, msglen, 0);
        closesocket(clientSocket);
	}

    closesocket(serverSocket);
    WSACleanup();

    return 0;

}