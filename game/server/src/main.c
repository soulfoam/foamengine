#include <server/server.h>

int
main(int argc, char *argv[])
{
    sv_setup(1337);
    sv_start();
    sv_cleanup();

    return 0;
}

