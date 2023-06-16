#include <foam/core/network/network.h>

FE_RETURN_CODE
fe_network_send(sock fd, const u8 *data, size_t data_len, const struct sockaddr_in *addr)
{
    ssize_t res = 0;
#ifndef _WIN32
    res = sendto(fd,
                 data,
                 data_len,
                 0,
                 (const struct sockaddr*) addr,
                 sizeof(*addr));
#else
    res = sendto(fd,
                 (char*)data,
                 data_len,
                 0,
                 (const struct sockaddr*) addr,
                 sizeof(*addr));
#endif

    if (res == -1) return FE_ERR_NETWORK_SEND;

    return FE_OK;
}


FE_RETURN_CODE 
fe_network_read(sock fd, u8 *data_dst, ssize_t *data_len_dst, struct sockaddr_in *from)
{

#ifndef _WIN32
    socklen_t addr_len = sizeof(*from);
    *data_len_dst = recvfrom(fd, 
			                 data_dst, 
			                 FE_NETWORK_PACKET_BUF_SIZE, 
			                 0, 
			                 (struct sockaddr*) from, 
			                 &addr_len);
#else

    i32 addr_len = sizeof(*from);
    *data_len_dst = recvfrom(fd, 
			                 (char*)data_dst, 
			                 FE_NETWORK_PACKET_BUF_SIZE, 
			                 0, 
			                 (struct sockaddr*) from, 
			                 &addr_len);

#endif

    if (*data_len_dst <= 0) return FE_ERR_NETWORK_READ;

    return FE_OK;
}

bool 
fe_network_addr_eq(const struct sockaddr_in *addr0, const struct sockaddr_in *addr1)
{
    return ((addr0->sin_addr.s_addr == addr1->sin_addr.s_addr) && (addr0->sin_port == addr1->sin_port));
}

static void
netcon_write_key_id_header(fe_netcon_t *nc)
{
    fe_io_write_u64(&nc->writer, nc->key);
    fe_io_write_u64(&nc->writer, nc->id);
}

static FE_RETURN_CODE
netcon_flush(fe_netcon_t *nc)
{
    FE_RETURN_CODE rc = fe_network_send(nc->fd, nc->writer.buffer, nc->writer.size, &nc->addr);
    fe_writer_clear(&nc->writer);
    return rc;
}

static void
netcon_send_ack(fe_netcon_t *nc)
{    
    netcon_write_key_id_header(nc);
    fe_io_write_u32(&nc->writer, FE_NETWORK_HEADER(0, 0, 1));
    netcon_flush(nc);
}

void
fe_netcon_make(fe_netcon_t *nc)
{
    nc->fd = -1;
	memset(&nc->addr, 0, sizeof(nc->addr));
    fe_writer_make_capacity(&nc->writer, FE_NETWORK_PACKET_BUF_SIZE, false);
    fe_writer_make_capacity(&nc->unreliable_buf, FE_NETWORK_PACKET_BUF_SIZE, false);
    fe_writer_make_capacity(&nc->reliable_buf, FE_NETWORK_PACKET_BUF_SIZE, false);
    //trigger first tick
    nc->resend_timer = FE_NETWORK_RESEND_TIME;
    nc->timeout_timer = 0;

    //1 because inital <= check would discard first packet thinking its old
    nc->seq_num         = 1;
    nc->rel_seq_num     = 1;
    nc->rem_seq_num     = 0;
    nc->rem_rel_seq_num = 0;

    nc->key = FE_INVALID_ID;
    nc->id  = FE_INVALID_ID;

}

void
fe_netcon_kill(fe_netcon_t *nc)
{
    fe_writer_kill(&nc->writer);
    fe_writer_kill(&nc->unreliable_buf);
    fe_writer_kill(&nc->reliable_buf);
}

void
fe_netcon_write(fe_netcon_t *nc, const u8 *data, size_t data_len, bool reliable)
{
    fe_writer_t *w = reliable ? &nc->reliable_buf : &nc->unreliable_buf;
    fe_io_write_data(w, data, data_len);
}

void
fe_netcon_tick(fe_netcon_t *nc)
{
    nc->timeout_timer++;

    if (!fe_writer_is_empty(&nc->reliable_buf))
    {
        nc->resend_timer++;
        if (nc->resend_timer >= FE_NETWORK_RESEND_TIME)
        {
            netcon_write_key_id_header(nc);
            fe_io_write_u32(&nc->writer, FE_NETWORK_HEADER(nc->rel_seq_num, 1, 0));
            fe_io_write_data(&nc->writer, nc->reliable_buf.buffer, nc->reliable_buf.size);
            //we dont add rel_seq_num here, only when we get an ack
            netcon_flush(nc); 
            nc->resend_timer = 0;
        }
    }

    if (!fe_writer_is_empty(&nc->unreliable_buf))
    {
        netcon_write_key_id_header(nc);
        fe_io_write_u32(&nc->writer, FE_NETWORK_HEADER(nc->seq_num, 0, 0));
        fe_io_write_data(&nc->writer, nc->unreliable_buf.buffer, nc->unreliable_buf.size);
        if (netcon_flush(nc) == FE_OK) nc->seq_num += 1;
        fe_writer_clear(&nc->unreliable_buf);
    }
}

bool
fe_netcon_is_timed_out(const fe_netcon_t *nc)
{
    return nc->timeout_timer >= FE_NETWORK_TIMEOUT_TIME;
}

bool
fe_netcon_process(fe_netcon_t *nc, fe_reader_t *reader)
{
    u32 seq = fe_io_read_u32(reader);

    u32 seq_num       = FE_NETWORK_HEADER_VALUE(seq);
    bool is_reliable  = FE_NETWORK_HEADER_FLAG(seq, FE_NETWORK_HEADER_FLAG_RELIABLE);
    bool is_ack       = FE_NETWORK_HEADER_FLAG(seq, FE_NETWORK_HEADER_FLAG_ACK);

    nc->timeout_timer = 0;

    //remote connection got our reliable packet, we can now send another
    if (is_ack)
    {
        nc->rel_seq_num += 1;
        fe_writer_clear(&nc->reliable_buf);
        nc->resend_timer = FE_NETWORK_RESEND_TIME;
        return false;
    }

    if (is_reliable)
    {
        //send ack no matter what
        netcon_send_ack(nc);

        if (seq_num <= nc->rem_rel_seq_num) return false;
        nc->rem_rel_seq_num = seq_num;
    }
    else
    {
        if (seq_num <= nc->rem_seq_num) return false;
        nc->rem_seq_num = seq_num;
    }

    return true;
}

FE_RETURN_CODE
fe_netcon_bind(fe_netcon_t *nc, u16 port)
{
#ifndef _WIN32
	if ((nc->fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		fe_log_fatal("NetCon_Bind: Failed to create socket");
        return FE_ERR_NETWORK_BIND;
	}
#else
	if ((nc->fd = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
	{
		fe_log_fatal("NetCon_Bind: Win32 failed to create socket");
        return FE_ERR_NETWORK_BIND;
	}
#endif
	memset(&nc->addr, 0, sizeof(nc->addr));

	nc->addr.sin_family      = AF_INET;
	nc->addr.sin_addr.s_addr = INADDR_ANY;
	nc->addr.sin_port        = htons(port);

	if (bind(nc->fd, (const struct sockaddr*)&nc->addr, sizeof(nc->addr)) < 0)
	{
		fe_log_fatal("NetCon_Bind: Failed to bind network socket on port %u", port);
        return FE_ERR_NETWORK_BIND;
	}

	i32 rc;
	i32 on = 1;
	rc = setsockopt(
			nc->fd,
			SOL_SOCKET,
			SO_REUSEADDR,
			(char *)&on,
			sizeof(on));

	if (rc < 0)
	{
		fe_log_error("NetCon_Bind: setsockopt() failed");
		close(nc->fd);
        return FE_ERR_NETWORK_BIND;
	}

#ifndef _WIN32
	rc = ioctl(nc->fd, FIONBIO, (char *)&on);
	if (rc < 0)
	{
		fe_log_error("NetCon_Bind: ioctl() failed");
		close(nc->fd);
        return FE_ERR_NETWORK_BIND;
	}
#else
	unsigned long mode = 1;
	rc = ioctlsocket(nc->fd, FIONBIO, &mode);
	if (rc < 0)
	{
		fe_log_error("NetCon_Bind: Win32 ioctlsocket() failed");
		close(nc->fd);
        return FE_ERR_NETWORK_BIND;
	}
    
#endif

	fe_log_info("NetCon_Bind: Successfully binded to port %u", port);

    return FE_OK;
}

FE_RETURN_CODE
fe_netcon_open(fe_netcon_t *nc, const char *ip, u16 port)
{
	nc->fd = socket(AF_INET, SOCK_DGRAM, 0);

#ifndef _WIN32
if (nc->fd == -1) 
{
	fe_log_warn("NetCon_Open: Failed to create socket");
	return FE_ERR_NETWORK_CONNECT;
}
#else
if (nc->fd == INVALID_SOCKET) 
{
	fe_log_warn("NetCon_Open: Win32 failed to create socket");
	return FE_ERR_NETWORK_CONNECT;
}
#endif
	memset(&nc->addr, 0, sizeof(nc->addr));
	nc->addr.sin_family = AF_INET;
	nc->addr.sin_port = htons(port);

	if (fe_inet_pton(AF_INET, &nc->addr.sin_addr, ip) == 0)
	{
		fe_log_warn("NetCon_Open: inet_pton failed to set server ip");
        return FE_ERR_NETWORK_CONNECT;
	}

#ifndef _WIN32
	i32 on = 1;
	i32 rc = setsockopt(
			nc->fd, 
			SOL_SOCKET, 
			SO_REUSEADDR, 
			(char *)&on, 
			sizeof(on));

	rc = ioctl(nc->fd, FIONBIO, (char *)&on);
#else
	unsigned long mode = 1;
	i32 res_ioctl = ioctlsocket(nc->fd, FIONBIO, &mode);
	if (res_ioctl != NO_ERROR) 
	{
		fe_log_warn("NetCon_Open: Win32 ioctl failed.");
        return FE_ERR_NETWORK_CONNECT;
	}
#endif

    return FE_OK;
}

void
fe_netcon_close(fe_netcon_t *nc)
{
#ifndef _WIN32
    close(nc->fd);	
#else
    closesocket(nc->fd);
#endif
}
