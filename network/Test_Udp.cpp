#include "KcpClient.h"
#include "KcpServer.h"
#include "EventLoop.h"
#include "KcpEvent.h"
#include "KcpSession.h"
#include "UdpPacket.h"
#include "UdpPacketFactory.h"

class KNetEvent : public KcpEvent
{
public:
	virtual KcpSession * createConnect()
	{
		return new KcpSession(this, 1000);
	}

	virtual void onAccept(KcpSession * conn){
		if (conn)
		{
			conn->sendMsg(1, "12345\0", 6);
		}
	};

	virtual void onConnect(KcpSession * conn){
		if (conn)
		{

		}
	};
	virtual void onClose(KcpSession * conn){

	};

	virtual void onMsg(KcpSession * conn, UdpPacket * pack){
		std::string s;
		(*pack) >> s;

		printf("%s\n", s.c_str());
		conn->sendMsg(1, pack);
	}

	virtual void onUdpTimeout(KcpSession * s)
	{
		printf("timeout ..............\n");
		server->closeSession(s);
	}

	KcpServer * server;
};

//#define UDP_CLIENT_TEST

int main()
{
	EventLoop::Instance()->init();
	udpPacketFactory = new UdpPacketFactory();

	//��ʼ���¼�ѭ��
	KNetEvent eve;
	EventLoop::Instance()->init();

#ifdef UDP_CLIENT_TEST
	KcpClient client(EventLoop::Instance(), &eve);
	client.createSession("127.0.0.1", 3001);

#else
	KcpServer server(EventLoop::Instance(), &eve);
	server.start("127.0.0.1", 3001);
	eve.server = &server;

#endif

	EventLoop::Instance()->Run();
	delete udpPacketFactory;

	system("pause");
	return 0;
}