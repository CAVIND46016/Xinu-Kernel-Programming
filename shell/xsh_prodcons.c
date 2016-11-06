#include <xinu.h>
#include <prodcons.h>
#include <stdlib.h>

//Definition for global variable 'n'
int n;                 
/*Now global variable n will be on Heap so it is accessible all the processes i.e. consume and produce*/

sid32 produced, consumed;

future *f_shared, *f_exclusive, *f_queue; 

shellcmd xsh_prodcons(int nargs, char *args[])
{
	produced=semcreate(1);
	consumed=semcreate(0);

	int count = 2000;

	if(nargs<2)
	{
		printf("Too few arguments!\n");
		return 1;
	}
	else if(nargs>2)
	{
		printf("Too many arguments!\n");
		return 1;
	}
	else if(nargs == 2)
	{
		if(strcmp(args[1],"-f") == 0)
		{
			f_exclusive = future_alloc(FUTURE_EXCLUSIVE);
			if( f_exclusive == NULL )
			{
				printf("Failed\n");
				return SYSERR;
			}
			resume( create(future_cons, 1024, 20, "fcons1", 1, f_exclusive) );
  			resume( create(future_prod, 1024, 20, "fprod1", 1, f_exclusive) );
		}
		else if(strcmp(args[1],"-fs") == 0)
		{
			f_shared = future_alloc(FUTURE_SHARED);
			if( f_shared == NULL )
			{
				printf("Failed\n");
				return SYSERR;
			}
			resume( create(future_cons, 1024, 20, "fcons2", 1, f_shared) );
			resume( create(future_cons, 1024, 20, "fcons3", 1, f_shared) );
  			resume( create(future_cons, 1024, 20, "fcons4", 1, f_shared) ); 
  			resume( create(future_cons, 1024, 20, "fcons5", 1, f_shared) );
			resume( create(future_prod, 1024, 20, "fprod2", 1, f_shared) );
		}
		else if(strcmp(args[1],"-fq") == 0)
		{
			f_queue=future_alloc(FUTURE_QUEUE);
			if( f_queue == NULL )
			{
				printf("Failed\n");
				return SYSERR;
			}
			resume( create(future_cons, 1024, 20, "fcons6", 1, f_queue) );
  			resume( create(future_cons, 1024, 20, "fcons7", 1, f_queue) );
  			resume( create(future_cons, 1024, 20, "fcons7", 1, f_queue) );
  			resume( create(future_cons, 1024, 20, "fcons7", 1, f_queue) );
  			resume( create(future_prod, 1024, 20, "fprod3", 1, f_queue) );
  			resume( create(future_prod, 1024, 20, "fprod4", 1, f_queue) );
  			resume( create(future_prod, 1024, 20, "fprod5", 1, f_queue) );
  			resume( create(future_prod, 1024, 20, "fprod6", 1, f_queue) );
  		}
		else if(args[1]!=NULL)
		{
			count= atoi(args[1]);

  			//create the process producer and consumer and put them in ready queue.
  			//Look at the definations of function create and resume in the system folder for reference.      
  			resume( create(producer, 1024, 20, "producer", 1, count));
  			resume( create(consumer, 1024, 20, "consumer", 1, count));
			return (0);	
		}
		else 
		{
			//count= atoi(args[1]);

  			//create the process producer and consumer and put them in ready queue.
  			//Look at the definations of function create and resume in the system folder for reference.      
  			resume( create(producer, 1024, 20, "producer", 1, count));
  			resume( create(consumer, 1024, 20, "consumer", 1, count));
			return (0);	
		}
	}
	return 0;
}