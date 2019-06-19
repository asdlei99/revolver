/*************************************************************************************
*filename: core_bandwidth_throttler.h
*
*to do:		��Ϣӳ�������
*Create on: 2012-05
*Author:	zerok
*check list:
*************************************************************************************/
#ifndef __CORE_BANDWIDTH_THROTTLER_H
#define __CORE_BANDWIDTH_THROTTLER_H

#include "base_typedef.h"
#include "base_namespace.h"
#include "base_singleton.h"
BASE_NAMESPACE_BEGIN_DECL

class CoreThrottler
{
public:
	CoreThrottler();
	~CoreThrottler();
	
	void	add_udp_packet(uint32_t packet_size, bool up = false);
	void	add_tcp_packet(uint32_t packet_size, bool up = false);

	void	reset();

	uint32_t	get_up_bandwidth() const {return up_bandwidth_;};
	uint32_t	get_down_bandwidth() const {return down_bandwidth_;};

	uint32_t	get_up_packet_count() const {return up_packet_count_;};
	uint32_t	get_down_packet_count() const {return down_packet_count_;};

	uint32_t	get_up_udp_count() const {return up_udp_count_;};
	uint32_t	get_down_udp_count() const {return down_udp_count_;};
	
	uint32_t	get_up_tcp_count() const {return up_tcp_count_;};
	uint32_t	get_down_tcp_count() const {return down_tcp_count_;};

	uint64_t	get_total_up_count() const {return total_up_count_;};
	uint64_t	get_total_down_count() const{return total_down_count_;};

protected:
	//����ͳ��
	uint32_t	up_bandwidth_;
	uint32_t	down_bandwidth_;
	//����ͳ��
	uint32_t	up_packet_count_;
	uint32_t	down_packet_count_;
	
	//UDP packet
	uint32_t	up_udp_count_;
	uint32_t	down_udp_count_;

	//TCP packet
	uint32_t	up_tcp_count_;
	uint32_t	down_tcp_count_;

	//�ܷ��ͱ�����
	uint64_t	total_up_count_;
	//�ܽ��ձ�����
	uint64_t	total_down_count_;
};

BASE_NAMESPACE_END_DECL

#define CREATE_THROTTLER		CSingleton<CoreThrottler>::instance
#define THROTTLER				CSingleton<CoreThrottler>::instance
#define DESTROY_THROTTLER		CSingleton<CoreThrottler>::destroy

#endif

/************************************************************************************/

