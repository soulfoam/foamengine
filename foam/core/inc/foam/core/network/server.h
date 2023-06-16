#pragma once

#include <foam/core/core.h>
#include <foam/core/ext/sugar.h>
#include <foam/core/ext/io.h>
#include <foam/core/network/network.h>
#include <foam/core/ext/genvec.h>

FE_DEFINE_HANDLE(fe_server_netcon_id);

typedef void (*fe_server_read_fn)(fe_server_netcon_id id, const u8 *data, u16 data_size);
typedef void (*fe_server_connect_fn)(fe_server_netcon_id id);
typedef void (*fe_server_disconnect_fn)(fe_server_netcon_id id);

void           fe_server_setup(f64 tick_rate, 
                               u32 component_count, 
                               u32 query_count, 
                               fe_server_connect_fn connect_fn, 
                               fe_server_disconnect_fn disconnect_fn, 
                               fe_server_read_fn read_fn);
void           fe_server_cleanup(void);
FE_RETURN_CODE fe_server_bind(u16 port);
void           fe_server_start(fe_core_tick_fn tick, fe_core_fixed_tick_fn fixed_tick);
void           fe_server_write(fe_server_netcon_id id, const u8 *buffer, u16 buf_size, bool reliable);
void           fe_server_kill_netcon(fe_server_netcon_id id);
void           fe_server_set_tick_fn(fe_core_tick_fn fn);
void           fe_server_set_connect_fn(fe_server_connect_fn fn);
void           fe_server_set_disconnect_fn(fe_server_disconnect_fn fn);
void           fe_server_set_read_fn(fe_server_read_fn fn);

