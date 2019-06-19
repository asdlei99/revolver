/*************************************************************************************
*filename:	core_log_macro.h
*
*to do:		����COREģ���LOG�궨��
*Create on: 2012-05
*Author:	zerok
*check list:
*************************************************************************************/
#ifndef __CORE_LOG_MACRO_H
#define __CORE_LOG_MACRO_H

#include "base_log.h"
#include "base_hex_string.h"

#ifdef WIN32
extern SingleLogStream core_log;
#else
extern BaseLogStream core_log;
#endif

#define CORE_DEBUG(arg)\
	DEBUG_TRACE(core_log, arg)

#define CORE_INFO(arg)\
	INFO_TRACE(core_log, arg)

#define CORE_WARNING(arg)\
	WARNING_TRACE(core_log, arg)

#define CORE_ERROR(arg)\
	ERROR_TRACE(core_log, arg)

#define CORE_FATAL(arg)\
	FATAL_TRACE(core_log, arg)

#define SET_LOG_LEVEL(arg)\
	core_log.set_trace_level(arg)

#endif
/************************************************************************************/

