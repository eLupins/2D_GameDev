#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include "simple_json_error.h"
#include "simple_json_string.h"

#if defined(WIN32)
#ifndef vsnprintf
#define vsnprintf _vsnprintf
#endif
#endif

static SJString _error = {0};

char *sj_get_error()
{
    return _error.text;
}

void sj_set_error(char *error,...)
{
    va_list ap;
    char *er;
    char buff[4];
    size_t length;

    // calculate error message length
    va_start(ap,error);
    length = vsnprintf(buff, sizeof(buff), error, ap);
    va_end(ap);

    //allocate buffer for error message
    er = (char *)malloc(sizeof(char) * (length + 2));
    memset(er,0,sizeof(char) * (length + 2));

    //write error to buffer
    va_start(ap,error);
    vsnprintf(er, sizeof(char) * (length + 2), error, ap);
    va_end(ap);
    
    // output error
    printf("%s\n",er);
    sj_string_set(&_error,er);
    
    //cleanup
    free(er);
}

/*eol@eof*/
