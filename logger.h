#include<stdio.h>

const char* __caller(const char *name) {return name;}
#define _caller() __caller(__func__)
#define caller() _caller()

typedef struct {
	int ret, write;
	FILE *f;
	const char *funcname;
} error_def;


void logger_start_f(error_def *err, const char *fname);
void log_clean(const char *fname);
void log_close(FILE *f) {close(f->_fileno);}
void my_log(error_def *err, int ret);
void my_log_(error_def *err,int ret, const char *name) {
	err->funcname = name;
	my_log(err, ret);
}

//#define my_log(err, ret) my_log_(err,ret, __func__) 
//#define my_log(err,ret) my_log_(err,ret,__func__)
#define mylog(err,ret) my_log_(err,ret, __func__)
