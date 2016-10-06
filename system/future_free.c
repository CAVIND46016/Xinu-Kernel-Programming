/* Assignment 4 */

/*Description:
	Free the allocated future. This function will simply free the space assigned to Future. Use freemem(). call to free the space.
    Parameters:
	future* f: future to free
    Return:
	syscall: SYSERR or OK
*/

#include<future.h>

syscall future_free(future* f)
{
	syscall sig = freemem(f, sizeof(f));
	f->state = FUTURE_EMPTY;
  	return sig;
}