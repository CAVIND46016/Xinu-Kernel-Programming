/* future_free.c - future_free */

#include <xinu.h>
#include<future.h>
/*------------------------------------------------------------------------
 *  future_free-frees the allocated future
 *------------------------------------------------------------------------
 */

typedef struct futent future;

 syscall future_free(future* f) 
{
	return freemem(f,sizeof(f));

}
	