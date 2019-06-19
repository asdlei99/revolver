/*************************************************************************************
*filename:	core_daemon_event.h
*
*to do:		����DAEMON�¼�֪ͨ�ӿ�
����
*Create on: 2012-05
*Author:	zerok
*check list:
*************************************************************************************/

#ifndef __CORE_DAEMON_EVENT_H
#define __CORE_DAEMON_EVENT_H

#include "base_inet_addr.h"

BASE_NAMESPACE_BEGIN_DECL

class IDaemonEvent
{
public:
	IDaemonEvent(){};
	virtual ~IDaemonEvent(){};

	//��������������֪ͨ
	virtual void on_add_server(uint32_t sid, uint8_t stype, uint16_t net_type, const Inet_Addr& tel_addr, const Inet_Addr& cnc_addr){};
	//��������������֪ͨ
	virtual void on_del_server(uint32_t sid, uint8_t stype){};
	//����ע����Ӧ����Ҫ�ǻ��SID����DAEMONͨ�ŵĵ�ַ�������˿�
	virtual void on_register(uint32_t sid, uint8_t stype, uint16_t net_type, const Inet_Addr& tel_addr, const Inet_Addr& cnc_addr){};
};

//���������ýӿڣ��ͻ��˿��Բ�ʵ��,���ӿ���Ҫ��ʵ�ֶ�SERVER ID, SERVER TYPE, SERVER IP�Ķ�ȡ�ͱ���,��Ҫ���ڱ���DAEMON���͹�������Ϣ
class IDaemonConfig
{
public:
	IDaemonConfig() {};
	virtual ~IDaemonConfig(){};

	//�����ó���ȡSERVER ID, SERVER TYPE, SERVER IP���浽SERVER_ID SERVER_TYPE�Ⱥ���
	virtual void read(){}; 
	virtual void write(){};
};

BASE_NAMESPACE_END_DECL
#endif	
/************************************************************************************/

