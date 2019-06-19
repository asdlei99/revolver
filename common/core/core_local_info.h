/*************************************************************************************
*filename:	core_local_info.h
*
*to do:		�������ؽڵ����Ϣ����Ҫ���������ط�����ID�����ط��������͡�IP��ַ���˿ڵȡ�
����
*Create on: 2012-05
*Author:	zerok
*check list:
*************************************************************************************/
#ifndef __CORE_LOCAL_INFO_H
#define __CORE_LOCAL_INFO_H

#include "base_namespace.h"
#include "base_typedef.h"
#include "base_inet_addr.h"
#include "base_singleton.h"

BASE_NAMESPACE_BEGIN_DECL

#define DEAFULT_SERVER_CERFITY		"you are pigs!!"

typedef struct Server_Info_t
{
	uint32_t	server_id;
	uint8_t		server_type;
	uint16_t	net_type;		//��ͨ�����š���ͨ�����磺�Ϻ���ͨ����0x0201,��һ���ֽڱ�ʾ���ڵ������ڶ����ֽڱ�ʾ������Ӫ������
	uint16_t	bind_port;		//���ط���󶨵�SOCKET�˿ڣ���������Ч��ַ�Ķ˿�
	Inet_Addr	tel_addr;		//���ŵ�ַ
	Inet_Addr	cnc_addr;		//��ͨ��ַ
	string		server_cerfity;	//������У�鴮
	Server_Info_t()
	{
		net_type = 0x0000;
		server_id = 0;
		server_type = 0;
		bind_port = 0;
		server_cerfity = DEAFULT_SERVER_CERFITY;
	}
}SERVER_INFO;

#define CREATE_LOCAL_SERVER		CSingleton<SERVER_INFO>::instance
#define LOCAL_SERVER			CSingleton<SERVER_INFO>::instance
#define DESTROY_LOCAL_SERVER	CSingleton<SERVER_INFO>::destroy

#define SERVER_ID \
	LOCAL_SERVER()->server_id

#define SERVER_TYPE \
	LOCAL_SERVER()->server_type

#define SERVER_NET_TYPE \
	LOCAL_SERVER()->net_type

#define SERVER_PORT \
	LOCAL_SERVER()->bind_port
	
#define TEL_IPADDR \
	LOCAL_SERVER()->tel_addr

#define CNC_IPADDR \
	LOCAL_SERVER()->cnc_addr

#define SERVER_CERFITY \
	LOCAL_SERVER()->server_cerfity

#define CHECK_TEL_NETTYPE(x) (((x) & 0x00ff) == 0x00)
#define CHECK_CNC_NETTYPE(x) (((x) & 0x00ff) == 0x01)
#define CHECK_CENTER_NETTYPE(x) (((x) & 0x00ff) == 0xff)

#define GET_NETTYPE(x) ((x) & 0x00ff)

BASE_NAMESPACE_END_DECL
#endif


/************************************************************************************/

