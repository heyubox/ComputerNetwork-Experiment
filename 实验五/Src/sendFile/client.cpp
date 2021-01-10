#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable : 4996)
#define BUF_SIZE 1024

int main() {
    //�������ļ��������ļ��Ƿ��ܴ����ɹ�
    char filename[100] = { 0 };  //�ļ���
    printf("Input filename to save: ");
    scanf("%s",&filename);
    FILE* fp = fopen(filename, "wb");  //�Զ����Ʒ�ʽ�򿪣��������ļ�
    if (fp == NULL) {
        printf("Cannot open file, press any key to exit!\n");
        system("pause");
        exit(0);
    }

    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));
    sockAddr.sin_family = PF_INET;
    sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    sockAddr.sin_port = htons(1234);
    connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));

    //ѭ���������ݣ�ֱ���ļ��������
    char buffer[BUF_SIZE] = { 0 };  //�ļ�������
    int nCount;
    while ((nCount = recv(sock, buffer, BUF_SIZE, 0)) > 0) {
        fwrite(buffer, nCount, 1, fp);
    }
    puts("File transfer success!");

    //�ļ�������Ϻ�ֱ�ӹر��׽��֣��������shutdown()
    fclose(fp);
    closesocket(sock);
    WSACleanup();
    system("pause");
    return 0;
}