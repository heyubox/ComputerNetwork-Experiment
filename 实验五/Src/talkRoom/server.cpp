#include <stdio.h>

#include <windows.h>
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable : 4996)
SOCKADDR_IN cAddr = { 0 };
int len = sizeof cAddr;
SOCKET clientSocket[1024];
int count = 0;
char idcat[1024] = {0};

void tongxin(int idx){
	char buff[1024];
	int r;
	while (1){
		r = recv(clientSocket[idx], buff, 1023, NULL);
		if (r > 0){
			buff[r] = 0;
			printf("%d:%s\n", idx, buff);
			//�㲥����
			itoa( idx,idcat, 1024);
			strcat(buff, " from ");
			strcat(buff, idcat);
			for (int i = 0; i < count; i++){
				if (i == idx)
					continue;
				send(clientSocket[i], buff, strlen(buff), NULL);
			}
		}
	}
}

int main(){
	//1 ����Э��汾
	WSADATA wsaData;
	//���� WSAStartup() �������г�ʼ������ָ�� WinSock �淶�İ汾��
	WSAStartup(MAKEWORD(2,2), &wsaData);
	if (LOBYTE(wsaData.wVersion) != 2 ||
		HIBYTE(wsaData.wVersion) != 2){
		printf("����Э��汾ʧ��!\n");
		return -1;
	}
	printf("����Э��ɹ�!\n");
	//2 ����socket
	//��socket Ҳ����Ϊ���ļ���һ�֣�����ͨ�ļ��Ĳ���û�������������������ݴ����������Ȼ����ʹ�����ļ� I/O ��صĺ�����������Ϊ����̨�����֮���ͨ�ţ�ʵ���������� socket �ļ����໥��д��
	SOCKET serverSocket =  socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (SOCKET_ERROR == serverSocket){
		printf("����socketʧ��!\n");
		WSACleanup();
		return -2;
	}
	printf("����socket�ɹ�!\n");

	//3 ����Э���ַ��
	SOCKADDR_IN addr = { 0 };
	addr.sin_family = AF_INET;//Э��汾��ʹ��IPv4��ַ
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//���Լ���ip
	addr.sin_port = htons(10086);//0 - 65535     10000���� �˿ں�
	//os�ں� ����������  ��ռ�õ�һЩ�˿�   80  23  

	//4 ��
	//bind() �������׽������ض���IP��ַ�Ͷ˿ڰ�������ֻ��������������IP��ַ�Ͷ˿ڵ����ݲ��ܽ����׽��ִ������ͻ���Ҫ�� connect() �����������ӡ�
	int r = bind(serverSocket, (sockaddr*)&addr, sizeof addr);
	if (-1 == r){
		printf("bindʧ��!\n");
		closesocket(serverSocket);
		WSACleanup();
		return -2;
	}
	printf("bind�ɹ�!\n");

	//5 ���� ������Ϊ10
	//ͨ�� listen() �����������׽��ֽ��뱻������״̬
	//listen() ֻ�����׽��ֽ������״̬����û���������տͻ�������listen() ����Ĵ�������ִ�У�ֱ������ accept()��
	//accept() ����������ִ�У�������벻�ܱ�ִ�У���ֱ�����µ���������
	r = listen(serverSocket, 10);
	if (-1 == r){
		printf("listenʧ��!\n");
		closesocket(serverSocket);
		WSACleanup();
		return -2;
	}
	printf("listen�ɹ�!\n");
	char assin[1024];

	//6 �ȴ��ͻ�������    ����    β������
	//�ͻ���Э���ַ��
	while (1){
		clientSocket[count] = accept(serverSocket, (sockaddr*)&cAddr, &len);
		if (SOCKET_ERROR == clientSocket[count]){
			printf("������崻���!\n");
			//8 �ر�socket
			closesocket(serverSocket);
			//9 ���Э����Ϣ
			WSACleanup();
			return -2;
		}
		printf("�пͻ������ӵ��������ˣ�%s!\n", inet_ntoa(cAddr.sin_addr));
		itoa(count, assin, 1024);
		send(clientSocket[count],assin, strlen(assin), NULL);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)tongxin,
			(char*)count, NULL, NULL);
		
		count++;
	}
	return 0;
}