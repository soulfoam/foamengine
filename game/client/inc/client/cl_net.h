#pragma once

#include <foam/foam_client.h>

void cl_net_on_connect(void);
void cl_net_on_disconnect(void);
void cl_net_on_read(const u8 *data, u16 data_len);
