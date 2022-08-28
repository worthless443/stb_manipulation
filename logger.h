#include<stdio.h>

const char* __caller(const char *name) {return name;}
#define _caller() __caller(__func__)
#define caller() _caller()

typedef struct {
	int ret;
	FILE *f;
} error_def;


FILE *logger_start_f(const char *fname);
void my_log(FILE *f, int ret, int write);
void log_clean(const char *fname);
void log_close(FILE *f) {close(f->_fileno);}
