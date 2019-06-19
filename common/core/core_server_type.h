/*************************************************************************************
*filename:	core_server_type.h
*
*to do:		�������������
*Create on: 2012-05
*Author:	zerok
*check list:
*************************************************************************************/
#ifndef __CORE_SERVER_TYPE_H
#define __CORE_SERVER_TYPE_H

#include "base_typedef.h"
const char* GetServerName(uint8_t stype);
typedef enum EServerType
{
	eClient					= 0x00,
	eDaemon_Server			= 0x01,
	//һ�����ӵķ�������
	eSample_Server			= 0x02,
	//todo:������д������SERVER���ͣ����ֱ���С��0xff�����Һ�ǰ������Ͳ��ظ�
	eMax_Server_Type
}EServerType;
#endif
/************************************************************************************/

