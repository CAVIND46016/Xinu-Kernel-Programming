/* Assignment 4 */

/*Description:
	Get the value of a future set by an operation and change the state of future from FUTURE_VALID to FUTURE_EMPTY.
    Parameters:
        future *f: future on which to wait for and obtain value
	int *value: variable in which to obtain the value from a future
    Return:
	syscall: SYSERR or OK
*/

#include<future.h>

syscall future_get(future *f, int *value)
{
	f->pid = getpid();
	suspend(f->pid);

 	if (f->state == FUTURE_VALID || f->state == FUTURE_EMPTY)
  	{
    		f->state = FUTURE_WAITING; 
		*value = f->value;
    		return OK;
 	}

  	return SYSERR;
}