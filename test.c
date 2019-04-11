/*
 * test2.c
 *
 * usage: ./test source-file 
 */


#include <stdio.h>
#include <stdlib.h>



int
main(int argc, char *argv[])
{
	
	char *a = malloc(112);
	char *b = malloc(112);
	char *c = malloc(112);

	printf("a is: %p \n", a);
	printf("b is: %p \n", b);
	printf("c is: %p \n", c);

}
