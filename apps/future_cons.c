#include <xinu.h>
#include<future.h>

typedef struct futent future;

uint future_cons(future *fut) 
{
	int i, status;
  	status = future_get(fut, &i);
  	if (status < 1) 
	{
    		printf("future_get failed\n");
    		return SYSERR;
  	}
  	printf("it produced %d\n", i);
  	return OK;
}