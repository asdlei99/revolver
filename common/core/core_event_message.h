/*************************************************************************************
*filename:	core_event_message.h
*
*to do:		����TCP�Ͽ�������֪ͨ��Ϣ
*Create on: 2012-05
*Author:	zerok
*check list:
*************************************************************************************/

#ifndef __CORE_EVENT_MESSAGE_H
#define __CORE_EVENT_MESSAGE_H

#include "base_packet.h"
#include "core_message_map_decl.h"

BASE_NAMESPACE_BEGIN_DECL

#define TCP_CONNECT_EVENT 0x00000000

class TCP_Connect_Message : public CBasePacket
{
public:
	TCP_Connect_Message()
	{
	};

	~TCP_Connect_Message()
	{
	};

	virtual void release_self()
	{
		delete this;
	}

protected:
	//������뺯��
	virtual void	Pack(BinStream& strm) const
	{
	};

	//���뺯��
	virtual void	UnPack(BinStream& strm)
	{
	};
};

#define TCP_CLOSE_EVENT	0xffffffff
class TCP_Close_Message : public CBasePacket
{
public:
	TCP_Close_Message(){};
	virtual ~TCP_Close_Message(){};

	virtual void release_self()
	{
		delete this;
	}

protected:
	//������뺯��
	virtual void	Pack(BinStream& strm) const
	{
	};

	//���뺯��
	virtual void	UnPack(BinStream& strm)
	{
	};
};


//��Ϣ��ӳ���ϵ
MESSAGEMAP_DECL_BEGIN(TCP_EVENT)
MESSAGEMAP_REGISTER(TCP_CONNECT_EVENT, new TCP_Connect_Message())
MESSAGEMAP_REGISTER(TCP_CLOSE_EVENT, new TCP_Close_Message())
MESSAGEMAP_DECL_END()

BASE_NAMESPACE_END_DECL

#endif
/************************************************************************************/
