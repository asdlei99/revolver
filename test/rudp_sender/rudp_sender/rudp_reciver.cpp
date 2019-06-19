#include "revolver/base_reactor_instance.h"
#include "reciver_thread.h"
#include "udp_handler.h"
#include "rudp_listen_handler.h"
#include "rudp/rudp_interface.h"
#include "rudp_connection.h"
#include "stat_packet.h"

#include <iostream>


using namespace std;

uint16_t port = 0;
void init()
{
	srand(time(NULL));

	port = rand() % 2000 + 1000;;

	init_socket();

	REACTOR_CREATE();
	REACTOR_INSTANCE()->open_reactor(20000);

	init_rudp_socket();

	CREATE_RECV_THREAD();
	CREATE_RECV_UDP();

	Inet_Addr local_addr(INADDR_ANY, port);
	RECV_UDP()->open(local_addr);

	//����һ��������
	CREATE_RUDP_LISTEN();
	RUDP()->attach_listener(RUDP_LISTEN());

	RECV_THREAD()->start();

}

void destroy()
{
	RECV_THREAD()->terminate();

	RUDP()->attach_listener(NULL);
	RUDP()->destroy();

	RECV_UDP()->close();

	REACTOR_INSTANCE()->close_reactor();

	DESTROY_RECV_UDP();
	DESTROY_RUDP_LISTEN();
	destroy_rudp_socket();

	DESTROY_RECV_THREAD();
	REACTOR_DESTROY();
	destroy_socket();
}

int main(int agc, char* argv[])
{
	cout << "start rudp sender... " << endl;

	init();

	StatPacketObj	stat;

	RUDPConnection *conn = NULL;
	while(true)
	{
		char c = getchar();

		if(c == 'e')
		{
			if (conn != NULL && (conn->get_state() == RUDP_CONN_CONNECTING || conn->get_state() == RUDP_CONN_CONNECTED))
				conn->close();

			break;
		}
		else if(c == 'c')
		{
			Inet_Addr local_addr(INADDR_ANY, port);	//���ذ󶨵�RUDP�����˿�
			Inet_Addr remote_addr("172.16.2.20", 2020);	//Զ�˰ﶥ��rudp�����˿�,rudp_recv�ĵ�ַ!!!!!!

			//for(int32_t i = 0; i < 1; i ++)
			{
				conn = new RUDPConnection();
				conn->set_stat(&stat);

				conn->connect(local_addr, remote_addr);
			}
		}
	}

	destroy();

	return 0;
}
