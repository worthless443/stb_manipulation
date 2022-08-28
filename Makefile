OBJS=lib.o
MAIN=main

all : $(OBJS) $(MAIN) 
$(OBJS): %.o : %.c 
	gcc -c -I./ $^ -o $@ 
$(MAIN): % : %.c
	gcc -I./ $^ $(OBJS) -lm -o $@
clean:
	rm -rf *.o $(MAIN)

