#include <foam/foam_client.h>
#include <client/client.h>

int
main(int argc, char **argv)
{
    cl_setup();
    fe_engine_start(cl_tick, cl_fixed_tick);
    cl_cleanup();

    return 0;
}


