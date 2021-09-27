#include<iostream>
#include <muduo/base/Logging.h>


#include"muduo/net/EventLoop.h"

#include"codec.h"
#include"ChatServer.h"
using namespace std;
int main()
{
	LOG_INFO << "pid = " << getpid();
	EventLoop loop;
	ChatServer server(&loop, InetAddress(2021));
	server.start();
	loop.loop();
	cout << " Server running finished!" << endl;
    return 0;
}