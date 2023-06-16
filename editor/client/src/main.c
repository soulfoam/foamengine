#include <foam/foam_client.h>
#include <client/editor.h>

int
main(int argc, char **argv)
{
    editor_setup();
    fe_engine_start(editor_tick, editor_fixed_tick);
    editor_cleanup();

    return 0;
}


