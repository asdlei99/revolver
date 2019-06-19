/*************************************************************************************
*filename:	core_daemon_client.h
*
*to do:		����CORE�Ŀ�ܣ���Ҫ�Ǳ��ؽڵ���Ϣ����UDP/TCP��������Daemon Client����
����
*Create on: 2012-05
*Author:	zerok
*check list:
*************************************************************************************/
#ifndef __CORE_DAEMON_CLIENT_H
#define __CORE_DAEMON_CLIENT_H

#include "base_namespace.h"
#include "base_typedef.h"
#include "core_cmd_target.h"
#include "base_inet_addr.h"
#include "core_udp_handler.h"
#include "core_msg_.h"

#include <map>

BASE_NAMESPACE_BEGIN_DECL

class IDaemonEvent;
class IDaemonConfig;

typedef std::map<uint8_t, uint8_t>		FocusSet;

class CDaemonClient : public ICmdTarget, 
					  public CEventHandler
{
public:
	enum {
		eDaemon_Idle		= 0x01,
		eStunning			= 0x02,
		eDaemon_Connecting	= 0x03,
		eDaemon_Connected	= 0x04,
		eDaemon_Disconnect	= 0x05,
		eDaemon_Finished	= 0x06
	};

public:
	CDaemonClient();
	~CDaemonClient();
	
	void attach_event(IDaemonEvent * daemon_event, IDaemonConfig* config);

	void init();

	//������ֹͣ
	void start(bool wan = false);
	void stop();

	//���һ����֪����
	void add_focus(uint8_t type);

	//��ʱ���ӿ�
	virtual int32_t handle_timeout(const void *act, uint32_t timer_id);

	//��Ϣ��������
	int32_t	on_daemon_pong(CBasePacket* packet, uint32_t sid, const Inet_Addr& remote_addr);
	int32_t on_register_res(CBasePacket* packet, uint32_t sid);
	int32_t on_add_server(CBasePacket* packet, uint32_t sid);
	int32_t on_del_server(CBasePacket* packet, uint32_t sid);
	//����ͨ���¼�
	int32_t connect_event(CBasePacket* packet, uint32_t sid, CConnection* connection);
	int32_t disconnect_event(CBasePacket* packet, uint32_t sid, CConnection* connection);

	int32_t	send_node_state(const string& node_info);

protected:
	uint32_t set_timer(uint32_t delay = 12000);
	void	 cancel_timer();

	bool	 is_focus(uint8_t type);

	void	 send_daemon_stun();
	void	 star_daemon_tcp();

protected:
	uint8_t			state_;
	uint32_t		timer_id_;
	IDaemonEvent*	daemon_event_;
	IDaemonConfig*	config_;

	//DAEMON SERVER����Ϣ
	uint32_t		daemon_sid_;	
	uint8_t			daemon_stype_;
	Inet_Addr		daemon_tel_addr_;
	Inet_Addr		daemon_cnc_addr_;

	FocusSet		focus_;

	//̽�⹫����ַUDP
	bool			wan_;
	bool			init_flag_;
	CoreUDPHandler	daemon_udp_;
	uint32_t		reconnect_count_;

	DECLARE_MSGCALL_MAP()
};

BASE_NAMESPACE_END_DECL

#endif
/************************************************************************************/
