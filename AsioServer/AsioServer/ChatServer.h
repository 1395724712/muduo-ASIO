#pragma once

#include<set>
#include<string>

#include"codec.h"
#include"muduo/net/TcpServer.h"

using namespace std;
using namespace muduo;
using namespace muduo::net;
class ChatServer
{
public:
	ChatServer(EventLoop* loop, const InetAddress& listenAddr);

	void start()
	{ server_.start(); }
private:

	void onMessage(const TcpConnectionPtr& conn);

	void onStringMessage(const TcpConnectionPtr& conn, const string& msg, Timestamp receiveTime);
	
	TcpServer server_;
	LengthHeaderCodec codec_;
	set<TcpConnectionPtr> connectionList_;
};

