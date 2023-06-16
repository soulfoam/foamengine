#include <core/core.h>
#include <foam/foam_core.h>
#include <server/server.h>
#include <server/sv_net.h>

typedef struct
{

} SV_Server;

static SV_Server sv;

static void
fixed_tick(void)
{

}

static void
tick(void)
{
    usleep(10);
}

void
sv_setup(u16 port)
{
    fe_server_setup(20, SERVER_COMPONENT_COUNT, SERVER_QUERY_COUNT, sv_net_on_connect, sv_net_on_disconnect, sv_net_on_read);
    fe_server_bind(port);

    co_setup();
}

void
sv_cleanup(void)
{
    fe_server_cleanup();
    co_cleanup();
}

void
sv_start(void)
{
    fe_server_start(tick, fixed_tick);
}

