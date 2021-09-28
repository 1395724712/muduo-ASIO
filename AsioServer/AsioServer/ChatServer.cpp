#include "ChatServer.h"

#include"muduo/base/Logging.h"

using namespace std::placeholders;
ChatServer::ChatServer(EventLoop * loop, const InetAddress & listenAddr):
server_(loop,listenAddr,"ChatServer"),
//todo:为什么codec要在这里初始化，和explicit有什么关系吗
//explicit要求该对象只能显式初始化，而没有默认初始化
codec_(bind(&ChatServer::onStringMessage, this, _1, _2, _3))
{
	server_.setConnectionCallback(bind(&ChatServer::onMessage, this, _1));
	server_.setMessageCallback(bind(&LengthHeaderCodec::onMessage, &codec_, _1, _2, _3));
}

void ChatServer::onMessage(const TcpConnectionPtr & conn)
{
	LOG_INFO << " Connection: " << conn->peerAddress().toIpPort() << " —> "
		<< conn->localAddress().toIpPort() << " is "
		<< (conn->connected() ? "UP" : "DOWN");
	if (conn->connected())
	{
		assert(connectionList_.find(conn) == connectionList_.end());
		connectionList_.insert(conn);
	}
	else
	{
		assert(connectionList_.find(conn) != connectionList_.end());
		connectionList_.erase(conn);
	}
}

void ChatServer::onStringMessage(const TcpConnectionPtr & conn, const string & msg, Timestamp receiveTime)
{
	for (auto connection : connectionList_)
		if(conn!=connection)
			codec_.send(connection, msg);
}
