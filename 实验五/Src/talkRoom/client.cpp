#include <stdio.h>

#include <windows.h>
#pragma warning(disable : 4996)
//#include <graphics.h>//easyX
#pragma comment(lib, "ws2_32.lib")

SOCKET clientSocket;
HWND hWnd;
int count = 0;
char myid[1024] = {0};
void jieshou(){
	char recvBuff[1024];
	int r;
	while (1){
		r = recv(clientSocket, recvBuff, 1023, NULL);
		if (r > 0){
			recvBuff[r] = 0;
			printf("%s\n", recvBuff);
			//outtextxy(0, count * 20, recvBuff);
			count++;
		}
	}
}

int main(){
	//��ʼ������
	//hWnd = initgraph(300, 400, SHOWCONSOLE);

	//1 ����Э��汾
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (LOBYTE(wsaData.wVersion) != 2 ||
		HIBYTE(wsaData.wVersion) != 2){
		printf("����Э��汾ʧ��!\n");
		return -1;
	}
	printf("����Э��ɹ�!\n");
	//2 ����socket
	clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (SOCKET_ERROR == clientSocket){
		printf("����socketʧ��!\n");
		WSACleanup();
		return -2;
	}
	printf("����socket�ɹ�!\n");

	//3 ��ȡ������Э���ַ��
	SOCKADDR_IN addr = { 0 };
	addr.sin_family = AF_INET;//Э��汾
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//���Լ���ip
	addr.sin_port = htons(10086);//0 - 65535     10000����
	//os�ں� ����������  ��ռ�õ�һЩ�˿�   80  23  

	//4 ���ӷ�����
	int r = connect(clientSocket, (sockaddr*)&addr, sizeof addr);
	if (r == -1){
		printf("���ӷ�����ʧ��!\n");
		return -1;
	}
	printf("���ӷ������ɹ�!\n");

	//5 ͨ��
	char buff[1024];
	CreateThread(NULL, NULL, 
		(LPTHREAD_START_ROUTINE)jieshou, 
		NULL, NULL, NULL);
	int firstConn = 1;
	
	while (1){
		if (firstConn) {
			firstConn = 0;
			char recvBuff[1024];
			r = recv(clientSocket, recvBuff, 1023, NULL);
			if (r > 0) {
				recvBuff[r] = 0;
				strcat(myid, recvBuff);
			}
		}
		memset(buff, 0, 1024);
		printf("����˵ɶ:\n");
		scanf("%s", buff);
		send(clientSocket, buff, strlen(buff), NULL);
	}






	return 0;
}