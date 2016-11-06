/* future_get.c - future_get */

#include <xinu.h>
#include<future.h>
/*------------------------------------------------------------------------
 *  future_get-Get the value of a future set by an operation and change the state of future from FUTURE_VALID or FUTURE_EMPTY to FUTURE_WAITING
 *------------------------------------------------------------------------
 */

typedef struct futent future;
sid32 sem1;

 syscall future_get(future *f, int *value) {
	
	pid32 pid;
	
	if(f->flag==FUTURE_EXCLUSIVE)
	{
		if(f->state==FUTURE_VALID)
		{
			*value=*(f->value);
			f->state=FUTURE_EMPTY;
			return OK;	
		 }
		if(f->state==FUTURE_EMPTY)
		{	
			 f->state = FUTURE_WAITING; 
			f->pid=getpid();
			suspend(f->pid);
			*value=*(f->value);
			f->state=FUTURE_EMPTY;
			return OK;
		}
		else
			return SYSERR;
	}
		
	else if(f->flag==FUTURE_SHARED)
	{
		if(f->state==FUTURE_VALID)
		{
			*value=*(f->value);
			f->state=FUTURE_EMPTY;
			return OK;
		}	

		else if(f->state==FUTURE_EMPTY||f->state==FUTURE_WAITING)
		{
			f->state = FUTURE_WAITING;
			pid=getpid();
			qinsert(&f->get_queue,pid);
			suspend(pid);
			*value=*(f->value);
			f->state=FUTURE_EMPTY;
			return OK;
		}
	
	}
		
	else 
	{
		if(f->set_queue.count > 0)
		{
			pid=qdelete(&f->set_queue);
			resume(pid);
			*value=*(f->value);
		}
		else
		{
			qinsert(&f->get_queue,getpid());
			suspend(getpid());
			*value=*(f->value);		
		}
		return OK;
	}
}