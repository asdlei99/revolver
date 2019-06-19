/*************************************************************************************
*filename: core_tcp_disconnect_notify.h
*
*to do:		�ڵ����ͨ��ӿڣ���������������ֹͣ�����ж�
*Create on: 2012-05
*Author:	zerok
*check list:
*************************************************************************************/
#ifndef __CORE_TCP_DISCONNECT_NOTIFY_H
#define __CORE_TCP_DISCONNECT_NOTIFY_H

#include "base_namespace.h"
#include "base_typedef.h"

BASE_NAMESPACE_BEGIN_DECL

class ICoreServerNotify
{
public:
	//DAEMON�ͱ��ط�������˷����������Ӷ�����
	virtual void on_server_dead(uint32_t sid, uint8_t stype) = 0;
};

BASE_NAMESPACE_END_DECL

#endif
/************************************************************************************/
