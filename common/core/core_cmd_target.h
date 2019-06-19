/*************************************************************************************
*filename: core_cmd_target.h
*
*to do:		��Ϣӳ�������
*Create on: 2012-05
*Author:	zerok
*check list:
*************************************************************************************/
#ifndef __CORE_CMD_TARGET_H
#define __CORE_CMD_TARGET_H

#include "base_namespace.h"
#include "base_typedef.h"
#include "base_inet_addr.h"
#include <map>

using namespace std;

BASE_NAMESPACE_BEGIN_DECL
class CBasePacket;
class CConnection;

struct CMD_MESSAGE_ENTRY;
class CTargetMessageManager;

class ICmdTarget
{
public:
	ICmdTarget();
	virtual ~ICmdTarget();

	//������ִ�������,���磺SID,mssage class, connection����ȵ�
	virtual int32_t on_event(uint32_t msg_id, uint32_t sid, CBasePacket* packet, CConnection* connection);
	//����UDP��Ϣ
	virtual int32_t on_event(uint32_t msg_id, uint32_t sid, CBasePacket* packet, const Inet_Addr& remote_addr);

protected:
	virtual CTargetMessageManager* get_message_map() = 0;
};

typedef void (ICmdTarget::*TARGET_CALL)(void);
typedef map<uint32_t, CMD_MESSAGE_ENTRY>	CMD_MESSAGE_MAP;

typedef struct CMD_MESSAGE_ENTRY
{
	uint32_t	msg_id;		//��Ϣ
	uint32_t	sig;		//������ʽ
	TARGET_CALL	fun;		//��������
}CMD_MESSAGE_ENTRY;

//�����������ص�ӳ�䣬������Ϣ��Ļ���Ч�ʾ�������
class CTargetMessageManager
{
public:
	CTargetMessageManager();
	~CTargetMessageManager();

	void		register_callback(uint32_t msg_id, uint32_t sig, TARGET_CALL fun);

	CMD_MESSAGE_ENTRY* get_callback(uint32_t msg_id);

protected:
	CMD_MESSAGE_MAP	message_map_;
};

BASE_NAMESPACE_END_DECL

#endif
/************************************************************************************/
