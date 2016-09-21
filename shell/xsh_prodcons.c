/*Assignment 2*/

#include <xinu.h>
#include <prodcons.h>
#include<stdlib.h>

/*-----------------------------------------------------------------------------------------------------------------------------------
 * xsh_prodcons- This file will contain the code pertaining to prodcons command
 *------------------------------------------------------------------------------------------------------------------------------------
 */

int n;                 //Definition for global variable 'n'
/*Now global variable n will be on Heap so it is accessible to all the processes i.e. consume and produce*/

shellcmd xsh_prodcons(int nargs, char *args[])
{
 	//Argument verifications and validations
 	int count = 2000;             //default value

  	//check args[1] if present assign value to count
	if(nargs  == 2)
	{
		count = (atoi)(args[1]);
	}
	else if(nargs > 2)
	{
		fprintf(stderr," Too many arguments provided.\n");
		return 1;
	}

  	//create the process producer and consumer and put them in ready queue.
  	//Look at the definations of function create and resume in the system folder for reference.      

  	resume( create(producer, 1024, 20, "producer", 1, count));
  	resume( create(consumer, 1024, 20, "consumer", 1, count));
  	return (0);
}
