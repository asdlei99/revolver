/*************************************************************************************
*filename:	core_tcp_listener.h
*
*to do:		����TCP�����߶��󣬸���ȫ�ֵ�TCP�������,�������ӵ�
*Create on: 2012-05
*Author:	zerok
*check list:
*************************************************************************************/
#ifndef __CORE_TCP_LISTENER_H
#define __CORE_TCP_LISTENER_H

#include "base_namespace.h"
#include "base_sock_acceptor.h"
#include "base_event_handler.h"

BASE_NAMESPACE_BEGIN_DECL

class CCoreTCPListener : public CEventHandler
{
public:
	CCoreTCPListener(); 
	virtual ~CCoreTCPListener();

	bool			is_open() const;

	BASE_HANDLER get_handle() const;

	int32_t open(const Inet_Addr& local_addr);
	int32_t close();

	int32_t handle_input(BASE_HANDLER handle);
	int32_t handle_output(BASE_HANDLER handle);
	int32_t handle_close(BASE_HANDLER handle, ReactorMask close_mask);
	int32_t handle_exception(BASE_HANDLER handle);

protected:
	CSockAcceptor acceptor_;
};

BASE_NAMESPACE_END_DECL

#endif
/************************************************************************************/
