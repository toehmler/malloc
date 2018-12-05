my-malloc.so: my-malloc.c
	gcc -g -Wall -pedantic -rdynamic -shared -fPIC -o my-malloc.so my-malloc.c

.phony: clean gdb_ls_l
clean:
	rm -rf my-malloc.so

gdb_ls_l:
	gdb --args env LD_PRELOAD=./my-malloc.so ls -l

