#include <math.h>
#include <time.h>
#include <foam/core/ecs/ecs.h>
#include <foam/core/core.h>
#include <foam/core/ext/io.h>
#include <foam/core/ext/vector.h>
#include <foam/core/ext/timer.h>
#include <foam/core/ext/hashmap.h>
#include <foam/core/network/server.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#ifndef _WIN32
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/time.h>
#include <sys/timeb.h>
#include <sys/poll.h>
#else
#include <winsock2.h>
#include <ws2tcpip.h>
#endif
#include <sys/types.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>
#include <errno.h>
#include <fcntl.h>

typedef struct
{
    fe_netcon_t net;
    fe_genvec(fe_netcon_t) users;

    fe_core_tick_fn       tick_fn;
    fe_core_fixed_tick_fn fixed_tick_fn;

    fe_server_read_fn       read_fn;
    fe_server_connect_fn    connect_fn;
    fe_server_disconnect_fn disconnect_fn;

} fe_server_t;

static fe_server_t sv;

static fe_server_netcon_id
add_user(sock fd, const struct sockaddr_in *addr, u64 key)
{
    fe_netcon_t *cl        = NULL;
    fe_server_netcon_id nc = (fe_server_netcon_id){0};
    fe_genvec_add_and_get(sv.users, nc.id, cl);
    fe_netcon_make(cl);
    cl->fd   = fd;
    cl->addr = *addr;
    cl->key  = key;
    cl->id   = nc.id;

    return nc;
}

static void
del_user(fe_server_netcon_id nc)
{
    if (sv.disconnect_fn) sv.disconnect_fn(nc);
    fe_genvec_del(sv.users, nc.id, fe_netcon_kill);
}

static fe_server_netcon_id
get_user_id_by_addr(const struct sockaddr_in *addr)
{
    fe_genvec_for_each(sv.users, user)
    {
        if(fe_network_addr_eq(&user->addr, addr))
        {
            return (fe_server_netcon_id){fe_genvec_for_each_cur_id(sv.users)};
        }
    }

    return (fe_server_netcon_id){FE_INVALID_ID};
}

static void
server_process(void)
{
	struct sockaddr_in from;
    u8 buffer[FE_NETWORK_PACKET_BUF_SIZE];
    ssize_t buf_size = 0;
    FE_RETURN_CODE rc = fe_network_read(sv.net.fd, buffer, &buf_size, &from);
    if (rc != FE_OK) return; 
    
    fe_server_netcon_id con = (fe_server_netcon_id){FE_INVALID_ID};

    fe_reader_t reader;
    fe_reader_init_buffer(&reader, buffer, buf_size);
    u64 key = fe_io_read_u64(&reader);
    u64 id  = fe_io_read_u64(&reader);
    if (key == FE_INVALID_ID && id == FE_INVALID_ID && FE_INVALID_HANDLE_ID(get_user_id_by_addr(&from)))
    {
        key = fe_random_u64();
        con = add_user(sv.net.fd, &from, key);
        if (sv.connect_fn) sv.connect_fn(con);
    }
    else
    {
        con.id = id;
    }

    fe_netcon_t *nc = fe_genvec_get(sv.users, con.id);
    if (nc && nc->key == key)
    {
        if (fe_netcon_process(nc, &reader))
        {
            fe_log("SERVER: [%s] read %u bytes", fe_network_sockaddr_to_string_s(&from), (u16)buf_size - reader.offset);
            if (sv.read_fn) sv.read_fn(con, reader.buffer + reader.offset, reader.size - reader.offset);
        }
    }
    else
    {
        fe_log_warn("SERVER: [%s] sent wrong key [%lu] or id [%lu]", fe_network_sockaddr_to_string_s(&from), key, id);
    }

}

static void
server_tick(void)
{
    server_process();
    if (sv.tick_fn) sv.tick_fn();
}

static void
server_fixed_tick(void)
{
    fe_genvec_for_each(sv.users, cl)
    {
        fe_netcon_tick(cl);
        if (fe_netcon_is_timed_out(cl))
        {
            del_user((fe_server_netcon_id){fe_genvec_for_each_cur_id(sv.users)});
        }
    }

    if (sv.fixed_tick_fn) sv.fixed_tick_fn();

}

void           
fe_server_setup(f64 tick_rate, 
                u32 component_count, 
                u32 query_count, 
                fe_server_connect_fn connect_fn, 
                fe_server_disconnect_fn disconnect_fn, 
                fe_server_read_fn read_fn)
{
    fe_netcon_make(&sv.net);
    sv.users     = FE_GENVEC_EMPTY;

    sv.connect_fn    = connect_fn;
    sv.disconnect_fn = disconnect_fn;
    sv.read_fn       = read_fn;

    fe_core_setup(tick_rate, component_count, query_count);
}

void
fe_server_cleanup(void)
{
    fe_netcon_close(&sv.net);
    fe_netcon_kill(&sv.net);
    fe_genvec_kill(sv.users, fe_netcon_kill);
    fe_core_cleanup();
}


FE_RETURN_CODE
fe_server_bind(u16 port)
{
    return fe_netcon_bind(&sv.net, port);
}

void
fe_server_start(fe_core_tick_fn tick, fe_core_fixed_tick_fn fixed_tick)
{
    sv.tick_fn = tick;
    sv.fixed_tick_fn = fixed_tick;
    fe_core_start(server_tick, server_fixed_tick);
}

void
fe_server_kill_netcon(fe_server_netcon_id id)
{
    del_user(id);
}

void
fe_server_write(fe_server_netcon_id nc, const u8 *buffer, u16 buf_size, bool reliable)
{
    fe_netcon_t *cl = fe_genvec_get(sv.users, nc.id);
    fe_netcon_write(cl, buffer, buf_size, reliable);
}

void           
fe_server_set_connect_fn(fe_server_connect_fn fn)
{
    sv.connect_fn = fn;
}

void           
fe_server_set_disconnect_fn(fe_server_disconnect_fn fn)
{
    sv.disconnect_fn = fn;
}

void           
fe_server_set_read_fn(fe_server_read_fn fn)
{
    sv.read_fn = fn;
}


