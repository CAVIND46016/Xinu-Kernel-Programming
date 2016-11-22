/* Assignment 4 */
 
 /*Description:
 	Free the allocated future. This function will simply free the space assigned to Future. Use freemem(). call to free the space.
     Parameters:
 	future* f: future to free
     Return:
 	syscall: SYSERR or OK
 */
 
 #include<future.h>

syscall future_free(future *f)
 {
 	int r;
	f->state = FUTURE_EMPTY;
 	r=freemem(f, sizeof(future));
 	if(r !=SYSERR)
 	{
 		return OK;
 	}
 	return SYSERR;
 } 