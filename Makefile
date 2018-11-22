CFLAGS=-g -Wall -pedantic 

PROGS=my-malloc.so

all: $(PROGS)

my-malloc.so: my-malloc.c
	gcc $(CFLAGS) -rdynamic -shared -fPIC -o $@ $^

.PHONY: clean
clean:
	rm -f * $(PROGS)
