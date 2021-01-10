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
			//广播数据
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
	//1 请求协议版本
	WSADATA wsaData;
	//调用 WSAStartup() 函数进行初始化，以指明 WinSock 规范的版本，
	WSAStartup(MAKEWORD(2,2), &wsaData);
	if (LOBYTE(wsaData.wVersion) != 2 ||
		HIBYTE(wsaData.wVersion) != 2){
		printf("请求协议版本失败!\n");
		return -1;
	}
	printf("请求协议成功!\n");
	//2 创建socket
	//，socket 也被认为是文件的一种，和普通文件的操作没有区别，所以在网络数据传输过程中自然可以使用与文件 I/O 相关的函数。可以认为，两台计算机之间的通信，实际上是两个 socket 文件的相互读写。
	SOCKET serverSocket =  socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (SOCKET_ERROR == serverSocket){
		printf("创建socket失败!\n");
		WSACleanup();
		return -2;
	}
	printf("创建socket成功!\n");

	//3 创建协议地址族
	SOCKADDR_IN addr = { 0 };
	addr.sin_family = AF_INET;//协议版本，使用IPv4地址
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//用自己的ip
	addr.sin_port = htons(10086);//0 - 65535     10000左右 端口号
	//os内核 和其他程序  会占用掉一些端口   80  23  

	//4 绑定
	//bind() 函数将套接字与特定的IP地址和端口绑定起来，只有这样，流经该IP地址和端口的数据才能交给套接字处理；而客户端要用 connect() 函数建立连接。
	int r = bind(serverSocket, (sockaddr*)&addr, sizeof addr);
	if (-1 == r){
		printf("bind失败!\n");
		closesocket(serverSocket);
		WSACleanup();
		return -2;
	}
	printf("bind成功!\n");

	//5 监听 缓冲区为10
	//通过 listen() 函数可以让套接字进入被动监听状态
	//listen() 只是让套接字进入监听状态，并没有真正接收客户端请求，listen() 后面的代码会继续执行，直到遇到 accept()。
	//accept() 会阻塞程序执行（后面代码不能被执行），直到有新的请求到来。
	r = listen(serverSocket, 10);
	if (-1 == r){
		printf("listen失败!\n");
		closesocket(serverSocket);
		WSACleanup();
		return -2;
	}
	printf("listen成功!\n");
	char assin[1024];

	//6 等待客户端连接    阻塞    尾生抱柱
	//客户端协议地址族
	while (1){
		clientSocket[count] = accept(serverSocket, (sockaddr*)&cAddr, &len);
		if (SOCKET_ERROR == clientSocket[count]){
			printf("服务器宕机了!\n");
			//8 关闭socket
			closesocket(serverSocket);
			//9 清除协议信息
			WSACleanup();
			return -2;
		}
		printf("有客户端连接到服务器了：%s!\n", inet_ntoa(cAddr.sin_addr));
		itoa(count, assin, 1024);
		send(clientSocket[count],assin, strlen(assin), NULL);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)tongxin,
			(char*)count, NULL, NULL);
		
		count++;
	}
	return 0;
}