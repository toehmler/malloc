my-malloc.so: my-malloc.c
	gcc -g -Wall -pedantic -rdynamic -shared -fPIC -o my-malloc.so my-malloc.c

.phony: clean permissions
clean:
	rm -rf my-malloc.so

# fixes VM shared folder permissions error
permissions:
	chmod 777 ../assignment3
	chmod 777 *
