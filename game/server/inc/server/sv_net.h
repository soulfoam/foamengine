#pragma once

#include <foam/foam_core.h>

void sv_net_on_connect(fe_server_netcon_id id);
void sv_net_on_disconnect(fe_server_netcon_id id);
void sv_net_on_read(fe_server_netcon_id id, const u8 *data, u16 data_len);
