#include "core_frame.h"
#include "core_udp_handler.h"
#include "core_connection.h"
#include "core_tcp_listener.h"
#include "core_daemon_client.h"
#include "base_reactor_instance.h"
#include "core_event_message.h"
#include "core_message_processor.h"
#include "core_message_map_decl.h"
#include "core_local_info.h"
#include "core_reactor_thread.h"
#include "core_connection_manager.h"
#include "base_log_thread.h"
#include "base_log.h"
#include "core_bandwidth_throttler.h"
#include "core_main.h"
#include "lzo_object.h"

BASE_NAMESPACE_BEGIN_DECL

ICoreFrame::ICoreFrame()
{
	daemon_client_ = NULL;
	listener_= NULL;
	udp_handler_ = NULL;
	dc_client_ = NULL;

	single_thread_ = false;
}

ICoreFrame::~ICoreFrame()
{

}

void ICoreFrame::release_handler()
{
	if(daemon_client_)
	{
		delete daemon_client_;
		daemon_client_ = NULL;
	}

	if(listener_)
	{
		delete listener_;
		listener_  = NULL;
	}

	if(udp_handler_)
	{
		delete udp_handler_;
		udp_handler_ = NULL;
	}

	if(dc_client_)
	{
		dc_client_->clear();

		delete dc_client_;
		dc_client_ = NULL;
	}
}

void ICoreFrame::init()
{
	init_socket();
	//�����ź�
	ignore_pipe();

	//����LZOѹ������
	LZO_CREATE();
	//����LOGϵͳ
	LOG_CREATE();
	LOG_THREAD_CREATE();

	CREATE_LOCAL_SERVER();

	REACTOR_CREATE();
	REACTOR_INSTANCE()->open_reactor(20000);

	CREATE_REACTOR_THREAD();

	//������Ϣӳ���ϵ
	CREATE_MSG_PROCESSOR();

	//�������ӹ�����
	CREATE_CONN_MANAGER();

	//����TCPͨ����Ϣ��ӳ��
	LOAD_MESSAGEMAP_DECL(TCP_EVENT);
	
	CREATE_THROTTLER();
	//�����ϲ��ʼ���ӿ�
	on_init();
	//����SELECT�߳�
	if(!single_thread_)
	{
		REACTOR_THREAD()->start();
		LOG_THREAD_INSTANCE()->start();
	}
}

void ICoreFrame::destroy()
{
	REACTOR_THREAD()->terminate();

	CONN_MANAGER()->close_all(); 
	
	usleep(50000);

	//ֹͣ���з���
	stop();

	//ֹͣ�߳�
	if(!single_thread_)
	{
		LOG_THREAD_INSTANCE()->terminate();
	}

	//ɾ��REACTOR
	REACTOR_INSTANCE()->close_reactor();

	//������Ϣ����
	DESTROY_MESSAGE_MAP_DECL();
	//�ͷŲ�����
	release_handler();
	on_destroy();

	DESTROY_THROTTLER();

	//ɾ�����ӹ���
	DESTROY_CONN_MANAGER();
	//ɾ���̶߳���
	DESTROY_REACTOR_THREAD();
	//ɾ��MSG_������
	DESTROY_MSG_PROCESSOR();
	//ɾ��ȫ�ֽڵ���Ϣ
	DESTROY_LOCAL_SERVER();
	//�ر�LOGϵͳ
	LOG_THREAD_DESTROY();
	LOG_DESTROY();
	
	REACTOR_DESTROY();
	//�ͷ�ѹ������
	LZO_DESTROY();

	destroy_socket();
}

void ICoreFrame::start(bool wan)
{
	if(daemon_client_)
	{
		daemon_client_->start(wan);
	}

	on_start();
}

void ICoreFrame::stop()
{
	on_stop();

	if(daemon_client_)
	{
		daemon_client_->stop();
	}

	if(listener_)
	{
		listener_->close();
	}

	if(udp_handler_)
	{
		udp_handler_->close();
	}
}

void ICoreFrame::bind_port(uint16_t port)
{
	Inet_Addr local_addr(INADDR_ANY, port);

	if(listener_ && !listener_->is_open())
	{
		listener_->open(local_addr);
	}

	if(udp_handler_ && !udp_handler_->is_open())
	{
		udp_handler_->open(local_addr);
	}
}

void ICoreFrame::create_daemon_client(IDaemonEvent * daemon_event, IDaemonConfig* config)
{
	if(daemon_client_ == NULL)
	{
		daemon_client_ = new CDaemonClient();
		daemon_client_->attach_event(daemon_event, config);
	}
}

void ICoreFrame::create_udp()
{
	if(udp_handler_ == NULL)
	{
		udp_handler_ = new CoreUDPHandler();
		udp_handler_->attach(MSG_PROCESSOR());

		CONN_MANAGER()->attach_udp(udp_handler_);
	}
}

void ICoreFrame::create_tcp_listener()
{
	if(listener_ == NULL)
		listener_ = new CCoreTCPListener();
}

void ICoreFrame::create_dc_client()
{
	if(dc_client_ == NULL)
	{
		dc_client_ = new CCoreDCClient();
	}
}

void ICoreFrame::attach_server_notify(ICoreServerNotify* notify)
{
	AttachServerNotify(notify);
}

void ICoreFrame::frame_run()
{
	if(single_thread_) //���߳�ģʽ
	{
		while(true)
		{
			REACTOR_INSTANCE()->event_loop();	
			wait_pid();
		}

		REACTOR_INSTANCE()->stop_event_loop();
	}
	else
	{
		core_main();
	}
}

uint32_t ICoreFrame::post_read_php(uint32_t dc_sid, CoreDCParam* param, const string& php, const string& php_param, bool ack /* = true */)
{
	uint32_t ret = 0;
	if(dc_client_ != NULL)
	{
		ret = dc_client_->post_dc_request(dc_sid, param, php, php_param, true, ack);
	}

	return ret;
}

uint32_t ICoreFrame::post_write_php(uint32_t dc_sid, CoreDCParam* param, const string& php, const string& php_param, bool ack /* = true */)
{
	uint32_t ret = 0;
	if(dc_client_ != NULL)
	{
		ret = dc_client_->post_dc_request(dc_sid, param, php, php_param, false, ack);
	}

	return ret;
}

CoreDCParam* ICoreFrame::cancel_dc_request(uint32_t exc_id)
{
	CoreDCParam* ret = NULL;
	if(dc_client_ != NULL)
	{
		ret = dc_client_->cancel_dc_request(exc_id);
	}

	return ret;
}

int32_t ICoreFrame::post_node_state(const string& node_info)
{
	int32_t ret = -1;
	if(daemon_client_ != NULL)
	{
		ret = daemon_client_->send_node_state(node_info);
	}

	return ret;
}


BASE_NAMESPACE_END_DECL

