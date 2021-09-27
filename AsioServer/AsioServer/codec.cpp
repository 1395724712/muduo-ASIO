#include "codec.h"
#include"muduo/base/Logging.h"
LengthHeaderCodec::LengthHeaderCodec(const StringMessageCallback messageCallback):
messageCallback_(messageCallback)
{
}

void LengthHeaderCodec::onMessage(const TcpConnectionPtr & conn, Buffer * buf, Timestamp receiveTime)
{
	while(buf->readableBytes()>=kHeaderLength_)
	{
		const void* data = buf->peek();
		int32_t hostLen = *static_cast<const int32_t*>(data);
		int32_t len = sockets::networkToHost32(hostLen);
		if(len>65536||len<0)
		{
			LOG_ERROR << " Invalid length: "<<len;
			conn->shutdown();
			break;
		}
		else if(buf->readableBytes()>=len+kHeaderLength_)
		{
			buf->retrieve(kHeaderLength_);
			string msg(buf->peek(), len);
			messageCallback_(conn, msg, receiveTime);
			buf->retrieve(len);
		}
		else
		{
			break;
		}
	}
}

void LengthHeaderCodec::send(const TcpConnectionPtr & conn, const string & msg)
{
	Buffer buf;
	buf.append(msg);
	int32_t len = static_cast<int32_t>(msg.size());
	len = sockets::hostToNetwork32(len);
	buf.prepend(&len, sizeof(len));
	conn->send(&buf);
}
