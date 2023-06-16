#include <server/sv_net.h>

void
sv_net_on_connect(fe_server_netcon_id id)
{
    fe_log("%lu dude connected", id);
}

void
sv_net_on_disconnect(fe_server_netcon_id id)
{
    fe_log("%lu dude disconnected", id);
}

void
sv_net_on_read(fe_server_netcon_id id, const u8 *data, u16 data_len)
{
   // fe_ioreader reader;
   // fe_io_reader_init_buffer(&reader, data, data_len);
   // char *lol = fe_io_read_string(&reader);
   // fe_log("%lu: sent %s %lu size", user, lol, data_len);
}
