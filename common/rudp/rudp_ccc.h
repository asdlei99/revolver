/*************************************************************************************
*filename:	rudp_ccc.h
*
*to do:		RUDP�������ʿ�������ʵ�ִ������������ʹ��ھ��ߡ�RTT�����
*Create on: 2013-04
*Author:	zerok
*check list:
*************************************************************************************/
#ifndef __RUDP_CCC_H_
#define __RUDP_CCC_H_

#include "base_typedef.h"
#include "base_namespace.h"
#include "rudp_packet.h"

BASE_NAMESPACE_BEGIN_DECL

//CCC���ٿ�����
class RUDPCCCObject
{
public:
	RUDPCCCObject();
	virtual ~RUDPCCCObject();

	void				init(uint64_t last_ack_id);
	void				reset();

	void				on_ack(uint64_t ack_seq);
	void				on_loss(uint64_t base_seq, const LossIDArray& loss_ids);
	void				on_timer(uint64_t now_ts);

public:
	uint32_t			get_send_window_size() const {return snd_cwnd_;};
	void				set_max_segment_size(uint16_t mss);

	void				set_rtt(uint32_t keep_live_rtt);
	uint32_t			get_rtt() const { return (uint32_t)(rtt_ * rtt_scale_); };

	uint32_t			get_rtt_var() const { return (uint32_t)(rtt_var_ * rtt_scale_); };

	void				add_resend();
	void				add_recv(uint32_t count);

protected:
	void				set_max_cwnd(uint32_t rtt);
private:
	//��ǰ���ʹ���
	uint32_t			snd_cwnd_;

	uint32_t			rtt_;
	uint32_t			rtt_var_;
	double				rtt_scale_;

	uint64_t			last_ack_id_;

	uint64_t			prev_on_ts_;

	bool				slow_start_;
	bool				loss_flag_;

	//��һ������RTT
	bool				rtt_first_;
	uint32_t			resend_count_;
	uint32_t			recv_count_;

	uint16_t			max_cwnd_;
	uint16_t			min_cwnd_;
	uint16_t			limit_cwnd_;

	uint32_t			print_count_;
};

BASE_NAMESPACE_END_DECL

#endif
/************************************************************************************/


