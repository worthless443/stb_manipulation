#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<execinfo.h>
#include<sys/mman.h>

#define GLOBAL_FILE 0x400000 //generic addr
			   //
			   //
static void fcopy(void *ptr1, void *ptr2) {
}

typedef struct {
	int ret, write;
	FILE *f;
	const char *funcname;
} error_def;

// c source 
static void setup_high_b_low_b(unsigned long int *hib, unsigned long int *lowb) {
  *hib = 0x80808080L;
  *lowb = 0x01010101L;
  if (sizeof (unsigned long int) > 4)
    {
      /* 64-bit version of the magic.  */
      /* Do the shift in two steps to avoid a warning if long has 32 bits.  */
      *hib = ((*hib << 16) << 16) | *hib;
      *lowb = ((*lowb << 16) << 16) | *lowb;
    }
}
// paritally c source 
static size_t get_size_ptr(const char *ptr) {
	const char *chr_ptr;
	size_t size,i=0;
	unsigned long int longword, hib, lowb;
	for(chr_ptr=ptr;((unsigned long int)chr_ptr & (sizeof(unsigned long int) - 1))!=0;++chr_ptr) 
		if(*chr_ptr=='\0') return chr_ptr - ptr;
	//for(i=0;*(int*)(ptr+i);i++)  if(*(int*)(ptr+i)<=256 && *(int*)(ptr+i)>0) size++;
	
	unsigned long int *longword_ptr = (unsigned long int*)ptr;
	setup_high_b_low_b(&hib, &lowb);
	while(1) {
		longword = *longword_ptr++;
		if(((longword - lowb) & ~longword & hib)!=0) {
			const char *cp = (const char*)(longword_ptr-1);
			if(sizeof(unsigned long int)>4)  
				for(int i=0;i<8;i++) if(cp[i]==0) return cp - ptr + i;
			else 
				for(int i=0;i<4;i++) if(cp[i]==0) return cp - ptr + i;
		}
	}
	return i;
}
static const char *_bt() {
	void *buf[100];
	char **strings;
	int nptrs = backtrace((void**)buf, 100);
	strings = backtrace_symbols((void**)buf, nptrs);
	int offset = nptrs - 7;
	const char *ret = strings[offset];
	free(strings);
	return ret;
}


int logger_start(const char *fname) {
	void *fp = mmap((void*)GLOBAL_FILE, sizeof(FILE), PROT_READ|PROT_WRITE, MAP_ANON|MAP_PRIVATE,0,0);
  	FILE *f = fopen(fname, "w");
  	if(fp!=(void*)GLOBAL_FILE || fp == MAP_FAILED) return 0;
  	memcpy(fp, f, sizeof(FILE)+16);
	return 1;
}

int err_log(const char *funcname, int ret) {
	char format[] = "\n%s() at %s, something returned wtih %d\n";
	char dig[64];
	sprintf(dig, "%d", ret);
	int size_d = get_size_ptr(dig);
	int size = get_size_ptr(funcname) + get_size_ptr(format) + get_size_ptr(_bt());
	char mess[size];
	sprintf(mess,format,funcname, _bt(), ret);
	FILE *f = (FILE*)GLOBAL_FILE;
	if(!fwrite(mess,1, size, f)) return 0;
	return 1;
}
void logger_flush() {
	fflush((FILE*)GLOBAL_FILE);
}
void logger_start_f(error_def *err, const char *fname) {
	err->f =  fopen(fname, "w");
}

void my_log(error_def *err, int ret) {
	char format[] = "\n%s() at %s, something returned wtih %d\n";
	char dig[64];
	sprintf(dig, "%d", ret);
	int size_d = get_size_ptr(dig);
	int size = get_size_ptr(err->funcname) + get_size_ptr(format) + get_size_ptr(_bt());
	char mess[size];

	sprintf(mess,format,err->funcname, _bt(), ret);
	if(err->write)
		fwrite(mess, 1, size, err->f);
	else
	   	printf("%s\n", mess);
}
void log_clean(const char *fname) {
	FILE *f = fopen(fname, "w");
	fwrite("", 1,1,f);
	close(f->_fileno);
}

void a(error_def *err) {
	my_log(err, 100);
	
}
#ifdef __main__   // pythonic practice
int main() {
	error_def err; 
	logger_start_f(&err, "nigger");
	err.write = 1;
	//FILE *f = (FILE*)FILE_ADDR;
	//printf("%d", ((FILE*)FILE_ADDR)->_fileno);
	//char *data = malloc(4*4);
	//memcpy(data,  "fuckerr", sizeof("fucker"));
	//printf("%ld\n", get_size_ptr(data));
	//my_log(f, "err: got 1", 0);
	//printf("%ld\n", sizeof(unsigned long int));
	mylog(&err, 1000);
	a(&err);
	//fread(data, 1,4,f);
}
#endif 
