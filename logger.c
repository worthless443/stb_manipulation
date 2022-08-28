#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<execinfo.h>
#include<sys/mman.h>
#define FILE_ADDR 0x400000 //generic addr
			   //

const char* __caller(const char *name) {return name;}
#define _caller() __caller(__func__)
#define caller() _caller()

typedef struct {
	int x,y;
} nigger_t ;

static void fcopy(void *ptr1, void *ptr2) {
	
}
static size_t get_size_ptr(const void *ptr) {
	size_t size,i;
	for(i=0;*(int*)(ptr+i);i++)  if(*(int*)(ptr+i)<256 && *(int*)(ptr+i)>0) size++;
	return i;
}
//void logger_start(const char *fname, int MAP_ADDR) {
//	FILE *ptr =  fopen(fname, "r");
//	void *addr = mmap((void*)MAP_ADDR, sizeof(FILE), PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANON, 0,0);
//	((FILE*)addr)->_flags = ptr->_flags;
//	((FILE*)addr)->_IO_read_ptr = ptr->_IO_read_ptr;
//	((FILE*)addr)->_IO_read_end = ptr->_IO_read_end;
//	((FILE*)addr)->_IO_read_base = ptr->_IO_read_base;
//	((FILE*)addr)->_IO_write_base = ptr->_IO_write_base;
//	((FILE*)addr)->_IO_write_ptr = ptr->_IO_write_ptr;
//	((FILE*)addr)->_IO_write_end = ptr->_IO_write_end;
//	((FILE*)addr)->_IO_save_base = ptr->_IO_save_base;
//	((FILE*)addr)->_IO_backup_base = ptr->_IO_backup_base;
//	((FILE*)addr)->_IO_save_end = ptr->_IO_save_end;
//	((FILE*)addr)->_markers = ptr->_markers;
//	((FILE*)addr)->_chain = ptr->_chain;
//	((FILE*)addr)->_fileno = ptr->_fileno;
//	((FILE*)addr)->_flags2 = ptr->_flags2;
//	((FILE*)addr)->_old_offset = ptr->_old_offset;
//	((FILE*)addr)->_cur_column = ptr->_cur_column;
//	((FILE*)addr)->_vtable_offset = ptr->_vtable_offset;
//	//memcpy(((FILE*)addr)->_shortbuf, ptr->_shortbuf, sizeof(ptr->_shortbuf));
//	((FILE*)addr)->_shortbuf[0] = ptr->_shortbuf[0];
//	printf("%d\n", ((FILE*)addr)->_shortbuf[0]);
//	((FILE*)addr)->_lock = ptr->_lock;
//	//printf("%d\n", ((FILE_t*)ptr)->_flag);
//}
//
//

const char *_bt() {
	char *buf[100];
	char **strings;
	int nptrs = backtrace((void**)buf, 100);
	strings = backtrace_symbols((void**)buf, 100);
	const char *ret = strings[1];
	//free(strings);
	return ret;
}

FILE *logger_start_f(const char *fname) {
	return fopen(fname, "w");
}
void my_log(FILE *f, int ret, int STDDEF) {
	char format[] = "\nin %s, something returned wtih %d\n";
	int size = sizeof(format)/sizeof(char) + get_size_ptr(_bt()) + 5;
	char mess[size];
	sprintf(mess,format, _bt(), ret);
	//printf("%s", mess);
	printf("%s\n", _bt());
	fwrite(mess, 1, size, f);
}

void log_clean(const char *fname) {
	FILE *f = fopen(fname, "w");
	fwrite("", 1,1,f);
	close(f->_fileno);
}
void log_close(FILE *f) {close(f->_fileno);}


void a(FILE *f) {
	my_log(f, 14, 0);
}
int main() {
	FILE *f = logger_start_f("fucker");
	//FILE *f = (FILE*)FILE_ADDR;
	//printf("%d", ((FILE*)FILE_ADDR)->_fileno);
	//char *data = malloc(4*4);
	//memcpy(data,  "fuckerr", sizeof("fucker"));
	//printf("%ld\n", get_size_ptr(data));
	//my_log(f, "err: got 1", 0);
	my_log(f, 223, 0);
	a(f);
	//fread(data, 1,4,f);
}
