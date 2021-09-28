#include <cstdio>
#include<iostream>
#include"muduo/base/Logging.h"
#include"muduo/net/EventLoopThread.h"
#include"ChatClient.h"
int main()
{
	LOG_INFO << " pid = " << getpid();
	EventLoopThread loopThread;

	ChatClient client(loopThread.startLoop(), InetAddress("192.168.123.202", 2021));
	client.connect();

	string msg;
	while(getline(std::cin,msg))
	{
		client.write(msg);
	}
	
    printf("hello from AsioClient!\n");
	getchar();
    return 0;
}