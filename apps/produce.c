#include <xinu.h>
#include <prodcons.h>

;void producer(int count) 
{
	
	int32 i;
	for(i=0;i<=count;i++)
	{
		wait(consumed);
		n=i;
		printf("\nProduced : %d",i);
		signal(produced);
	}

}
