/* xsh_hello.c - xsh_hello
     Assignment 1
*/

#include <xinu.h>
#include <string.h>
#include <stdio.h>

/*------------------------------------------------------------------------------------------------------------------------------------
 * xsh_hello- add a simple hello command which will take a single string argument.
 *-------------------------------------------------------------------------------------------------------------------------------------
 */
shellcmd xsh_hello(int nargs, char *args[]) {

	if(nargs > 2 )
	{
		fprintf(stderr, "Too many arguments provided");
		return 1; 
	}
	else if(nargs < 2)
	{
		fprintf(stderr, "At least one argument required.");
		return 1;
	}

	printf("Hello %s, Welcome to the world of Xinu!!", args[1]);

	return 0;
}
