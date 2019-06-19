/*************************************************************************************
*filename:	core_connection.h
*
*to do:		CORE���Ӷ��������ʵ��TCP���������շ���������
*Create on: 2012-05
*Author:	zerok
*check list:
*************************************************************************************/
#ifndef __CORE_CONNECTION_H
#define __CORE_CONNECTION_H

#include "base_bin_stream.h"
#include "base_event_handler.h"
#include "base_sock_stream.h"
#include "core_reciver_interface.h"
#include "base_block_buffer.h"

#include <list>
using namespace std;

BASE_NAMESPACE_BEGIN_DECL
//16K����
typedef CReciverBuffer_T<BinStream, CSockStream, 1024 * 4>		RBuffer_16K; 
typedef CSenderBuffer_T<BinStream, CSockStream, 1024 * 4>		SBuffer_16K;
//256K����
typedef CReciverBuffer_T<BinStream, CSockStream, 1024 * 256>	RBuffer_256K; 
typedef CSenderBuffer_T<BinStream, CSockStream, 1024 * 256>		SBuffer_256K;
//1M����
typedef CReciverBuffer_T<BinStream, CSockStream, 1024 * 1024>	RBuffer_1M;
typedef CSenderBuffer_T<BinStream, CSockStream, 1024 * 1024>	SBuffer_1M;
//10M����
typedef CReciverBuffer_T<BinStream, CSockStream, 1024 * 1280 * 8>	RBuffer_10M;
typedef CSenderBuffer_T<BinStream, CSockStream, 1024 * 1280 * 8>	SBuffer_10M;

#define __16K_BUFFER

#ifdef __10M_BUFFER
typedef RBuffer_10M		RBuffer;
typedef SBuffer_10M		SBuffer;
#endif

#ifdef __1M_BUFFER
typedef RBuffer_1M		RBuffer;
typedef SBuffer_1M		SBuffer;
#endif

#ifdef __256K_BUFFER
typedef RBuffer_256K	RBuffer;
typedef SBuffer_256K	SBuffer;
#endif

#ifdef __16K_BUFFER
typedef RBuffer_16K		RBuffer;
typedef SBuffer_16K		SBuffer;
#endif

class CCorePacket;
class CoreExpPacket;

class CConnection : public CEventHandler
{
public:
	enum 
	{
		CONN_IDLE,			//ƽ��״̬,�п�����δ����
		CONN_CONNECTING,	//����״̬
		CONN_CONNECTED,		//����״̬
	};

	CConnection();
	virtual ~CConnection();

public:
	void			 reset();
	CSockStream&	get_sock_stream();


	uint16_t		get_state() const {return state_;};
	void			set_state(uint16_t state);

	BASE_HANDLER	get_handle() const;
	void			set_handle(BASE_HANDLER handle);

	void			set_conn_ptr(void* ptr);
	void*			get_conn_ptr();

	uint32_t		get_index() const{return index_;};
	void			set_index(uint32_t index){index_ = index;};

	const Inet_Addr& get_remote_addr() const {return remote_addr_;};
	void			set_remote_addr(const Inet_Addr& remote_addr) {remote_addr_ = remote_addr;};


	//��ȡ�˵���Ϣ
	uint8_t			get_server_type() const{return server_type_;};
	void			set_server_type(uint8_t server_type){server_type_ = server_type;};	
	uint32_t		get_server_id() const {return server_id_;};
	void			set_server_id(uint32_t server_id){server_id_ = server_id;};

	//�¼��ӿ�
	int32_t			handle_input(BASE_HANDLER handle);
	int32_t			handle_output(BASE_HANDLER handle);
	int32_t			handle_close(BASE_HANDLER handle, ReactorMask close_mask);
	int32_t			handle_exception(BASE_HANDLER handle);
	int32_t			handle_timeout(const void *act, uint32_t timer_id);

	//����һ��TCP����
	int32_t			connect(const Inet_Addr& remote_addr);
	int32_t			connect(const Inet_Addr& src_addr, const Inet_Addr& dst_addr);
	//�ر�����
	void			close();
	void			extern_close();

	//��������
	int32_t			send(CCorePacket& packet, bool no_delay = false);
	int32_t			send(const string& bin_stream);

protected:
	int32_t			process(CCorePacket &packet, BinStream& istrm);
	void			process_ping(const CCorePacket &packet);
	int32_t			process_handshake(const CCorePacket &packet, BinStream& istrm);

	void			send_ping();
	void			send_handshake();

	void			check_connecting_state();
	void			buffer_reduce();

	uint32_t		set_timer(uint32_t timer_type, uint32_t delay = 60000); 
	void			cancel_timer();
	void			release_timer_act(const void* act);

	int32_t			heartbeat();
	
	void			generate_digest(uint32_t server_id, uint8_t server_type, string& digest_data);
protected:
	uint16_t		state_;
	uint32_t		timer_id_;

	CSockStream		sock_stream_;

	SBuffer			sbuffer_;			//����BUFFER
	RBuffer			rbuffer_;			//����BUFFER

	BinStream		istrm_;

	void*			conn_ptr_;			//���ӵ�������Ϣ

	uint32_t		index_;				//����ӳ������
	uint8_t			server_type_;		//0��ʾ�ͻ���
	uint32_t		server_id_;			//��ID

	uint32_t		timer_count_;		//��ʱ������

	string			conn_name_;
	bool			send_flag_;			//���ͱ��ı�־

	Inet_Addr		remote_addr_;
};

BASE_NAMESPACE_END_DECL

#endif
/************************************************************************************/

