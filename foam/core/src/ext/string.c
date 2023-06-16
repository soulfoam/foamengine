#include <string.h>
#include <foam/core/ext/string.h>
#include <foam/core/ext/logger.h>

char*
fe_string_replace(const char *s, const char *rep, const char *with) 
{
    char *result; 
    int i, cnt = 0; 

    size_t replen = strlen(rep); 
    size_t withlen = strlen(with); 
  
    for (i = 0; s[i] != '\0'; i++) 
    { 
        if (strstr(&s[i], rep) == &s[i]) 
        { 
            cnt++; 
  
            i += replen - 1; 
        } 
    } 
    
    if (cnt == 0) return NULL;
  
    result = malloc(i + cnt * (withlen - replen) + 1); 
  
    i = 0; 
    while (*s) 
    { 
        if (strstr(s, rep) == s) 
        { 
            strcpy(&result[i], with); 
            i += withlen; 
            s += replen; 
        } 
        else
        {
            result[i++] = *s++; 
        }
    } 
  
    result[i] = '\0'; 

    return result; 
}

char*
fe_string_remove(char *str, const char *sub) 
{
    size_t len = strlen(sub);
    if (len > 0) 
    {
        char *p = str;
        while ((p = strstr(p, sub)) != NULL) 
        {    
            memmove(p, p + len, strlen(p + len) + 1);
        }
    }

    return str;
}

char*
fe_string_null_terminate(const char *buffer, size_t buffer_len)
{
    char *s = malloc(buffer_len+1);
    s[buffer_len] = '\0';
    memcpy(s, buffer, buffer_len);

    return s;
}

void
fe_string_generate_random(char *dst, size_t len)
{
    len -= 1;

    char charset[] = "0123456789"
                     "abcdefghijklmnopqrstuvwxyz"
                     "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    while (len-- > 0) 
    {
        size_t index = (double) rand() / RAND_MAX * (sizeof charset - 1);
        *dst++ = charset[index];
    }
    *dst = '\0';
}

void
fe_string_copy(char *dst, const char *src, size_t len)
{ 
    strncpy(dst, src, len-1);
    dst[len] = '\0';
}


static void
print_bin(u32 n)
{
    u32 i;
    for (i = 1 << 31; i > 0; i = i / 2)
        (n & i)? printf("1"): printf("0");


    printf("\n");
}
