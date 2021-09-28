#include "ChatClient.h"

#include"muduo/base/Logging.h"

using namespace std::placeholders;
ChatClient::ChatClient(EventLoop* loop, const InetAddress& serverAddr):
	client_(loop,serverAddr,"ChatClient"),
	codec_(bind(&ChatClient::onStringMessageCallback,this,_1,_2,_3))
{
	client_.setConnectionCallback(bind(&ChatClient::onConnection, this, _1));
	client_.setMessageCallback(bind(&LengthHeaderCodec::onMessage, &codec_, _1, _2, _3));
	client_.enableRetry();
}

void ChatClient::write(const string& msg)
{
	MutexLockGuard lock(mutex_);
	if(connection_)
		codec_.send(connection_, msg);
}

void ChatClient::onConnection(const TcpConnectionPtr& conn)
{
	LOG_INFO << "Connection: " << conn->localAddress().toIpPort() << " -> "
		<< conn->peerAddress().toIpPort() << " is "
		<< (conn->connected() ? "UP" : "DOWN");

	MutexLockGuard lock(mutex_);
	if (conn->connected())
		connection_ = conn;
	else
		connection_.reset();
}

void ChatClient::onStringMessageCallback(const TcpConnectionPtr & conn, const string & msg, Timestamp receiveTime)
{
	printf("<<<%s\n",msg.c_str());
}


