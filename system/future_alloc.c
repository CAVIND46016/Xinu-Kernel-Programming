/* Assignment 4 */ 
 
 /*Description:
 	Allocates a new future (in the FUTURE_EMPTY state) in given mode. We will use getmem() call to allocate space for the new future.
      Parameters:
 	int future_flag: flag to be set for this future
      Return:
 	future: NULL or a valid future reference
 */
 
 #include<future.h>
 
 future* future_alloc(int future_flag)
 {
 	future *f 	= getmem(sizeof(future));
 	f->flag 		= future_flag;
 	f->state 	= FUTURE_EMPTY;
 	return f;
 } 