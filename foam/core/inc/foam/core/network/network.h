#pragma once

#include <foam/core/network/network_ext.h>

#define FE_NETWORK_HEADER_SIZE 4
#define FE_NETWORK_PACKET_BUF_SIZE (1024 + FE_NETWORK_HEADER_SIZE)
#define FE_NETWORK_SOCKADDR_STR_LEN 22

#define FE_NETWORK_HEADER_FLAG_RELIABLE 31
#define FE_NETWORK_HEADER_FLAG_ACK      30

#define FE_NETWORK_HEADER(num, rel, ack) (num | (rel << FE_NETWORK_HEADER_FLAG_RELIABLE) \
                                              | (ack << FE_NETWORK_HEADER_FLAG_ACK))

#define FE_NETWORK_HEADER_TOGGLE_FLAG(num, flag)   (num ^ (1 << flag))
#define FE_NETWORK_HEADER_SET_FLAG(num, flag, on)  (num | (on << flag))
#define FE_NETWORK_HEADER_FLAG(num, flag)          (num & (1 << flag))
#define FE_NETWORK_HEADER_VALUE(num)               (num & ~(1 << 31) & ~(1 << 30))

#define FE_NETWORK_RESEND_TIME 1 * 20 //in ticks
#define FE_NETWORK_TIMEOUT_TIME 10 * 20 //in ticks and this is hardcoded 20 bad

#ifndef _WIN32
typedef i16 sock;
#else
typedef SOCKET sock;
#endif

typedef struct
{
    sock fd;
	struct sockaddr_in addr;
    fe_writer_t writer;

    fe_writer_t unreliable_buf;
    fe_writer_t reliable_buf;
    u32 resend_timer;
    u32 timeout_timer;

    //two bit flags held on seq_num's.. MSB is if this packet contains reliable data, 
    //and MSB-1 is if we got the remote connections last reliable packet, 
    //as we can only have 1 reliable packet out at one time
    u32 seq_num; 
    u32 rel_seq_num; 

    //last sequence number gotten from remote connection
    u32 rem_seq_num; 
    u32 rem_rel_seq_num;

    u64 key;
    u64 id;

} fe_netcon_t;

static inline void
fe_network_sockaddr_to_string(const struct sockaddr_in *addr, char *dst)
{
    sprintf(dst, "%s:%u", inet_ntoa(addr->sin_addr), ntohs(addr->sin_port));
}

static inline char*
fe_network_sockaddr_to_string_s(const struct sockaddr_in *addr)
{
    static char ret[FE_NETWORK_SOCKADDR_STR_LEN];
    sprintf(ret, "%s:%u", inet_ntoa(addr->sin_addr), ntohs(addr->sin_port));
    return ret;
}


FE_RETURN_CODE fe_network_send(sock fd, const u8 *data, size_t data_len, const struct sockaddr_in *addr);
FE_RETURN_CODE fe_network_read(sock fd, u8 *data_dst, ssize_t *data_len_dst, struct sockaddr_in *from);
bool           fe_network_addr_eq(const struct sockaddr_in *addr0, const struct sockaddr_in *addr1);

void           fe_netcon_make(fe_netcon_t *nc);
void           fe_netcon_kill(fe_netcon_t *nc);
FE_RETURN_CODE fe_netcon_open(fe_netcon_t *nc, const char *ip, u16 port);
void           fe_netcon_close(fe_netcon_t *nc);
FE_RETURN_CODE fe_netcon_bind(fe_netcon_t *nc, u16 port);
void           fe_netcon_write(fe_netcon_t *nc, const u8 *data, size_t data_len, bool reliable);
void           fe_netcon_tick(fe_netcon_t *nc);
bool           fe_netcon_process(fe_netcon_t *nc, fe_reader_t *reader);
bool           fe_netcon_is_timed_out(const fe_netcon_t *nc);
