#include <stdlib.h>
#include <stdio.h>
#include <foam/core/ext/sugar.h>
#include <foam/core/config/config.h>
#include <exporter.h>

i32
main(i32 argc, char **argv)
{
	if (argc <= 1)
	{
		fe_log_error("Usage: fe_exporter input.feco");
		exit(1);
	}
	
	const char *input_file = argv[1];

    fe_export_config(input_file);

}

