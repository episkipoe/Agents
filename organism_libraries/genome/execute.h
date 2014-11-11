#ifndef EXECUTE_H
#define EXECUTE_H

#include <stdarg.h>

pid_t compile_and_execute(const char * code, const char * libraries, char ** args) ;
pid_t compile_and_execute(const char * code, const char * libraries, int n_args, va_list args) ;
pid_t compile_and_execute(const char * code, const char * libraries, int n_args, ...) ;

#endif
