#pragma once
//Author:wh
//Date:20210925
//Description:½âÂë
#include"muduo/net/TcpConnection.h"
#include"muduo/net/Buffer.h"
#include"muduo/net/Endian.h"

#include<string>

using namespace std;
using namespace muduo;
using namespace muduo::net;
class LengthHeaderCodec
{
public:
	typedef std::function<void(const TcpConnectionPtr&, const string& msg, Timestamp)> StringMessageCallback;

	explicit LengthHeaderCodec(const StringMessageCallback messageCallback);

	void onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp receiveTime);

	void send(const TcpConnectionPtr& conn, const string& msg);

private:

	const StringMessageCallback messageCallback_;
	const size_t kHeaderLength_ = sizeof(int32_t);
	
};

