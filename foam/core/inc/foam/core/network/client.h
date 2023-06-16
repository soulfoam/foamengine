#pragma once

#include <foam/core/ext/sugar.h>
#include <foam/core/ext/io.h>
#include <foam/core/network/network.h>

typedef void (*fe_client_read_fn)(const u8 *data, u16 data_size);
typedef void (*fe_client_connect_fn)(void);
typedef void (*fe_client_disconnect_fn)(void);

void fe_client_setup(void);
void fe_client_cleanup(void);

FE_RETURN_CODE fe_client_connect(const char *ip, u16 port);
void           fe_client_disconnect(void);

void fe_client_write(const u8 *buf, u16 buf_size, bool reliable);
void fe_client_process(void);
void fe_client_fixed_tick(void);
void fe_client_set_callbacks(fe_client_connect_fn connect, fe_client_disconnect_fn disconnect, fe_client_read_fn read);
void fe_client_set_connect_fn(fe_client_connect_fn cb);
void fe_client_set_disconnect_fn(fe_client_disconnect_fn cb);
void fe_client_set_read_fn(fe_client_read_fn cb);

