#pragma once

#include <stdlib.h>

char* fe_string_replace(const char *orig, const char *rep, const char *with);
char* fe_string_remove(char *str, const char *sub);
char* fe_string_null_terminate(const char *buffer, size_t buffer_len);
void  fe_string_generate_random(char *dst, size_t len);
void  fe_string_copy(char *dst, const char *src, size_t len);
