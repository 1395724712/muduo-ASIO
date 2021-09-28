#pragma once
#include"codec.h"
#include"muduo/net/TcpClient.h"

using namespace muduo;
using namespace muduo::net;
class ChatClient
{
public:
	ChatClient(EventLoop*loop, const InetAddress& serverAddr);

	void connect()
	{ client_.connect(); }

	void write(const string& msg);
private:

	void onConnection(const TcpConnectionPtr& conn);

	void onStringMessageCallback(const TcpConnectionPtr& conn, const string& msg, Timestamp receiveTime);
	
		
	TcpClient client_;
	LengthHeaderCodec codec_;
	MutexLock mutex_;
	TcpConnectionPtr connection_ GUARDED_BY(mutex_);
};

