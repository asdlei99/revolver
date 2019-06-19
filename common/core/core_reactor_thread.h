/*************************************************************************************
*filename:	core_reactor_thread.h
*
*to do:		��Ӧ�������߳�
*Create on: 2012-05
*Author:	zerok
*check list:
*************************************************************************************/
#ifndef __CORE_REACTOR_THREAD_H
#define __CORE_REACTOR_THREAD_H

#include "base_namespace.h"
#include "base_typedef.h"
#include "base_thread.h"
#include "base_singleton.h"

BASE_NAMESPACE_BEGIN_DECL

class CCoreThread : public CThread
{
public:
	CCoreThread();
	virtual ~CCoreThread();
	
	void execute();
};

#define CREATE_REACTOR_THREAD	CSingleton<CCoreThread>::instance
#define REACTOR_THREAD			CSingleton<CCoreThread>::instance
#define DESTROY_REACTOR_THREAD	CSingleton<CCoreThread>::destroy


BASE_NAMESPACE_END_DECL
#endif
/************************************************************************************/
