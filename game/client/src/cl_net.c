#include <client/cl_net.h>

void
cl_net_on_connect(void)
{
    fe_log("connected to server");
}

void
cl_net_on_disconnect(void)
{
    fe_log("disconnected from server");
}

void
cl_net_on_read(const u8 *data, u16 data_len)
{
    
}
