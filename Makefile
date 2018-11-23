my-malloc.so: my-malloc.c
	gcc -g -Wall -pedantic -rdynamic -shared -fPIC -o my-malloc.so my-malloc.c

.phony: clean
clean:
	rm -rf my-malloc.so