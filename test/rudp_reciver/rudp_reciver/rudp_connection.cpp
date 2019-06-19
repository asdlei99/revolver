#include "revolver/base_reactor_instance.h"
#include "rudp_connection.h"
#include "core/core_packet.h"

RUDPConnection::RUDPConnection()
{
	timer_id_ = 0;
	byte_count_ = 0;
	packet_count_ = 0;
	total_count_ = 0;
}

RUDPConnection::~RUDPConnection()
{
	reset();
}

void RUDPConnection::reset()
{
	cancel_timer();

	state_ = RUDP_CONN_IDLE;

	rbuffer_.reset();
	sbuffer_.reset();

	istrm_.rewind(true);

	rudp_sock_.close();

	byte_count_ = 0;
	packet_count_ = 0;
	total_count_ = 0;
}

void RUDPConnection::set_timer(uint32_t delay)
{
	if(timer_id_ == 0)
		timer_id_ = REACTOR_INSTANCE()->set_timer(this, NULL, delay);
}

void RUDPConnection::cancel_timer()
{
	if(timer_id_ != 0)
	{
		const void* act = NULL;
		REACTOR_INSTANCE()->cancel_timer(timer_id_, &act);
		timer_id_ = 0;
	}
}

void RUDPConnection::heartbeat()
{
	cout << "bandwidth = " << byte_count_ / 1024 << " KB/S, packet count = " << packet_count_ << "total count = " << total_count_ << endl;

	byte_count_ = 0;
	packet_count_ = 0;
}

int32_t RUDPConnection::handle_timeout(const void *act, uint32_t timer_id)
{
	if(timer_id == timer_id_)
	{
		timer_id_ = 0;

		switch(state_)
		{
		case RUDP_CONN_CONNECTING:
			state_ = RUDP_CONN_IDLE;
			this->close();
			break;

		case RUDP_CONN_CONNECTED:
			heartbeat();
			set_timer(1000);
			break;
		}
	}

	return 0;
}

int32_t RUDPConnection::connect(const Inet_Addr& src_addr, const Inet_Addr& dst_addr)
{
	if(rudp_sock_.open(src_addr) != 0)
	{
		return -1;
	}

	if(rudp_sock_.connect(dst_addr) != 0)
	{
		this->close();
		return -1;
	}

	rudp_sock_.set_option(4, RUDP_SEND_BUFFER);

	//��һ���¼���
	RUDP()->bind_event_handle(rudp_sock_.get_handler(), this);

	state_ = RUDP_CONN_CONNECTING;
	set_timer(23000);

	return 0;
}

void RUDPConnection::close()
{
	RUDP()->bind_event_handle(rudp_sock_.get_handler(), NULL);
	rudp_close_event(rudp_sock_.get_handler());
}

int32_t RUDPConnection::send(RUDPTestPacket& packet)
{
	int32_t ret = -1;
	BinStream *bin_strm = STREAMPOOL.pop_obj();
	bin_strm->rewind(true);
	*bin_strm << packet;

	if(sbuffer_.remaining_length() < bin_strm->data_size())
	{
		if(sbuffer_.length() < MAX_BUFFER_SIZE)//����TCP���ͻ�����,��ֹ������̫С��ɷ��Ͱ��쳣
		{
			sbuffer_.realloc_buffer(bin_strm->data_size());
		}
		else //���ͱ��Ķ���
		{
			bin_strm->rewind(true);
			STREAMPOOL.push_obj(bin_strm);
			return -1;
		}
	}

	if(sbuffer_.put(*bin_strm))
	{
		RUDP()->register_event(rudp_sock_.get_handler(), MASK_WRITE);
		ret = 0;
	}

	bin_strm->rewind(true);
	STREAMPOOL.push_obj(bin_strm);

	return ret;
}

int32_t RUDPConnection::rudp_accept_event(int32_t rudp_id)
{
	return 0;
}

int32_t RUDPConnection::rudp_input_event(int32_t rudp_id)
{
	if(state_ == RUDP_CONN_CONNECTING)
	{
		std::cout << "rudp connected!" << std::endl;
		return 0;
	}

	int32_t rc = 0;
	if(rbuffer_.remaining_length() == 0) //����TCP���ջ�����,��ֹ������̫С����հ��쳣
	{
		if(rbuffer_.length() < MAX_BUFFER_SIZE)
			rbuffer_.realloc_buffer(rbuffer_.length() - 1);
		else
		{
			return -1;
		}
	}

	rc = rbuffer_.recv(rudp_sock_);
	if(rc > 0)
	{
		byte_count_ += rc;
		bool split_flag = true;
		while(split_flag) //��Ϣ��װ
		{
			int32_t split_ret = rbuffer_.split(istrm_);
			if(split_ret == 0)
			{
				RUDPTestPacket packet;

				try{istrm_ >> packet; }
				catch(...)
				{
					this->close();
					return -2;
				}

				packet_count_ ++;
				total_count_++;

				process(&packet);
			}
			else if(split_ret < 0) //�Ƿ�����
			{
				this->close();
				return -2;
			}
			else //���ĳ��Ȳ����ж�
			{
				split_flag = false;
			}
		}
	}
	
	return 0;
}

int32_t RUDPConnection::rudp_output_event(int32_t rudp_id)
{
	if(state_ == RUDP_CONN_CONNECTING)
	{
		std::cout << "rudp connected!" << std::endl;
		cancel_timer();

		state_ = RUDP_CONN_CONNECTED;
		set_timer(1000);
	}

	if(sbuffer_.data_length() > 0)
		return sbuffer_.send(rudp_sock_);

	return 0;
}

int32_t RUDPConnection::rudp_close_event(int32_t rudp_id)
{
	std::cout << "close rudp !!" << std::endl;
	if(state_ != RUDP_CONN_IDLE)
	{
		reset();

		delete this;
	}
	else
	{
		//rudp_sock_.close();
	}

	return 0;
}

int32_t RUDPConnection::rudp_exception_event(int32_t rudp_id)
{
	std::cout << "exception rudp !!" << std::endl;
	if(state_ != RUDP_CONN_IDLE)
	{
		reset();

		delete this;
	}
	else
	{
		//rudp_sock_.close();
	}

	return 0;
}

void RUDPConnection::process(RUDPTestPacket* packet)
{
		send(*packet);
}










