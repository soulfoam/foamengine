#pragma once

#include <foam/core/ext/vector.h>
#include <foam/core/ext/sugar.h>
#include <foam/core/ext/io.h>
#include <util.h>
#include <conf.h>

void fe_export_skeleton(const char *input_path, const char *output_path, const char *config_path);
