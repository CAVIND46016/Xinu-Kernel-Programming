/* Assignment 4 */

/*Description:
	Set a value in a future and changes state from FUTURE_EMPTY or FUTURE_WAITING to FUTURE_VALID.
    Parameters:
	future *f: future in which to set the value
        int *value: result of an operation to be set as value in a future
    Return:
	syscall: SYSERR or OK
*/

#include <xinu.h>
#include<future.h>

/*------------------------------------------------------------------------
 *  future_set-Set a value in a future and changes state from FUTURE_EMPTY or FUTURE_WAITING to FUTURE_VALID
 *------------------------------------------------------------------------
 */

typedef struct futent future;

syscall future_set(future *f, int *value) 
{
	
	if(f->state==FUTURE_EMPTY || f->state==FUTURE_WAITING)
	{
		f->value=*value;
		f->state=FUTURE_VALID;
		ready(f->pid);
		return OK;
	}
	return SYSERR;
}