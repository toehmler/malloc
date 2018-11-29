my-malloc.so: my-malloc.c
	gcc -g -Wall -pedantic -rdynamic -shared -fPIC -o my-malloc.so my-malloc.c

.phony: clean permissions gdb_ls_l
clean:
	rm -rf my-malloc.so

# fixes VM shared folder permissions error
permissions:
	chmod 777 ../assignment3
	chmod 777 *

gdb_ls:
	gdb --args env LD_PRELOAD=./my-malloc.so ls

gdb_ls_l:
	gdb --args env LD_PRELOAD=./my-malloc.so ls -l

