#include <foam/core/network/client.h>
#include <foam/core/ext/sugar.h>
#include <foam/core/ext/io.h>
#include <foam/core/ext/vector.h>
#include <foam/core/ext/hashmap.h>

typedef struct
{
    fe_netcon_t net;

    bool connected;

    fe_client_read_fn       read_fn;
    fe_client_connect_fn    connect_fn;
    fe_client_disconnect_fn disconnect_fn;

} fe_client_t;

static fe_client_t cl;

void
fe_client_setup(void)
{
    fe_netcon_make(&cl.net);
    cl.connected     = false;
    cl.read_fn       = NULL;
    cl.connect_fn    = NULL;
    cl.disconnect_fn = NULL;

#ifdef _WIN32
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
	{
		fe_log("CLIENT: WSAStartup failed... Error Code : %d", WSAGetLastError());
		return;
	}
#endif
}

void
fe_client_cleanup(void)
{
    if (cl.connected) fe_client_disconnect();
#ifdef _WIN32
    WSACleanup();
#endif
    fe_netcon_kill(&cl.net);
}

FE_RETURN_CODE
fe_client_connect(const char *ip, u16 port)
{
    FE_RETURN_CODE rc = fe_netcon_open(&cl.net, ip, port);
	if (rc == FE_OK) fe_log_info("CLIENT: Attempting to connect to %s:%u", ip, port);
    return rc;
}

void
fe_client_disconnect(void)
{
    if (!cl.connected) return;

    fe_netcon_close(&cl.net);
    cl.connected = false;
    if (cl.disconnect_fn) cl.disconnect_fn();
}

void 
fe_client_write(const u8 *buf, u16 buf_size, bool reliable)
{
    fe_netcon_write(&cl.net, buf, buf_size, reliable);
}

void
fe_client_process(void)
{
	struct sockaddr_in from;
    u8 buffer[FE_NETWORK_PACKET_BUF_SIZE];
    ssize_t buf_size = 0;
    FE_RETURN_CODE rc = fe_network_read(cl.net.fd, buffer, &buf_size, &from);
    if (rc != FE_OK) return; 

    if (!fe_network_addr_eq(&from, &cl.net.addr)) 
    {
        fe_log_warn("CLIENT: Got packet from unknown source [%s]", fe_network_sockaddr_to_string_s(&from));
        return;
    }

    fe_reader_t reader;
    fe_reader_init_buffer(&reader, buffer, buf_size);
    u64 key = fe_io_read_u64(&reader);
    u64 id  = fe_io_read_u64(&reader);

    if (!cl.connected)
    { 
        if (cl.connect_fn) cl.connect_fn();
        cl.connected = true;
        cl.net.key = key;
        cl.net.id  = id;
    }

    if (fe_netcon_process(&cl.net, &reader))
    {
        fe_log("CLIENT: [%s] Read %u bytes", fe_network_sockaddr_to_string_s(&from), (u16)buf_size);
        if (cl.read_fn) cl.read_fn(reader.buffer + reader.offset, reader.size - reader.offset);

    }
}

void
fe_client_fixed_tick(void)
{
    fe_netcon_tick(&cl.net);
    if (fe_netcon_is_timed_out(&cl.net))
    {
        fe_client_disconnect();
    }
}

void 
fe_client_set_callbacks(fe_client_connect_fn connect, fe_client_disconnect_fn disconnect, fe_client_read_fn read)
{
    cl.connect_fn    = connect;
    cl.disconnect_fn = disconnect;
    cl.read_fn       = read;
}

void           
fe_client_set_connect_fn(fe_client_connect_fn con)
{
    cl.connect_fn = con;
}

void           
fe_client_set_disconnect_fn(fe_client_disconnect_fn dcon)
{
    cl.disconnect_fn = dcon;
}

void           
fe_client_set_read_fn(fe_client_read_fn read)
{
    cl.read_fn = read;
}

