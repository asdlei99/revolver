/*************************************************************************************
*filename:	core_frame.h
*
*to do:		����CORE�Ŀ�ܣ���Ҫ�Ǳ��ؽڵ���Ϣ����UDP/TCP��������Daemon Client����
			����
*Create on: 2012-05
*Author:	zerok
*check list:
*************************************************************************************/
#ifndef __CORE_FRAME_H
#define __CORE_FRAME_H

#include "base_namespace.h"
#include "base_typedef.h"
#include "core_daemon_client.h"
#include "core_dc_client.h"

BASE_NAMESPACE_BEGIN_DECL

class CoreUDPHandler;
class CCoreTCPListener;
class ICoreServerNotify;

class ICoreFrame
{
public:
	ICoreFrame();
	~ICoreFrame();

	void				init();
	void				destroy();

	void				start(bool wan = false);
	void				stop();

	void				release_handler();

	//CORE������к���
	void				frame_run();
	
	//DAEMON CLIENT���ط���õĵ�ַ����������󶨣������DAEMON
	void				bind_port(uint16_t port);

	//�������
	void				create_udp();
	void				create_tcp_listener();
	void				create_daemon_client(IDaemonEvent* daemon_event, IDaemonConfig* config = NULL);
	void				create_dc_client();
	void				attach_server_notify(ICoreServerNotify* notify);

	CoreUDPHandler*		get_udp_handler(){return udp_handler_;};
	
	uint32_t			post_write_php(uint32_t dc_sid, CoreDCParam* param, const string& php, const string& php_param, bool ack = true);
	uint32_t			post_read_php(uint32_t dc_sid, CoreDCParam* param, const string& php, const string& php_param, bool ack = true);

	CoreDCParam*		cancel_dc_request(uint32_t exc_id);
	//����ڵ�״̬��daemond������
	int32_t				post_node_state(const string& node_info);

	//�ṩ���ϲ���¼�
	virtual void		on_init() = 0;
	virtual void		on_destroy() = 0;
	
	virtual void		on_start() = 0;
	virtual void		on_stop() = 0;
	
	//����ӽ����Ƿ��˳�
	virtual void		wait_pid(){};

protected:
	CDaemonClient*		daemon_client_;
	CCoreTCPListener*	listener_;
	CoreUDPHandler*		udp_handler_;
	CCoreDCClient*		dc_client_;
	bool				single_thread_; //	ȫ���߳����
};

BASE_NAMESPACE_END_DECL

#endif
/************************************************************************************/


