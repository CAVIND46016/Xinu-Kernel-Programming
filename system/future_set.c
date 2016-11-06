/* future_set.c - future_set */

#include <xinu.h>
#include<future.h>
#include<prodcons.h>
/*------------------------------------------------------------------------
 *  future_set-Set a value in a future and changes state from FUTURE_EMPTY or FUTURE_WAITING to FUTURE_VALID
 *------------------------------------------------------------------------
 */

typedef struct futent future;


 syscall future_set(future *ff, int *value) 
{
	pid32 pid;
	
	 if(ff->flag==FUTURE_QUEUE)
	{		
		if(ff->get_queue.count > 0)
		{
			*(ff->value)=*value;
			pid=qdelete(&ff->get_queue);
			resume(pid);

		}
		else
		{
			qinsert(&ff->set_queue,getpid());
			suspend(getpid());
			*(ff->value)=*value;
		}
		return OK;
	}

	if(ff->state==FUTURE_EMPTY )
	{
		*(ff->value)=*value;
		ff->state=FUTURE_VALID;
		//ready(f->pid);
		return OK;
	}
	else if(ff->state==FUTURE_WAITING)
	{
		*(ff->value) = *value;
		ff->state=FUTURE_VALID;
		if(ff->flag==FUTURE_SHARED) 
		{
			pid=qdelete(&ff->get_queue);
			while(pid!=SYSERR)
			{
				resume(pid);
				pid=qdelete(&ff->get_queue);

			}
		}
		else if(ff->flag == FUTURE_QUEUE)
		{
			pid=qdelete(&ff->get_queue);
			resume(pid);
		}
		else
		{
			resume(ff->pid);
		}
		return OK;
	}
	else 
		return SYSERR;
}