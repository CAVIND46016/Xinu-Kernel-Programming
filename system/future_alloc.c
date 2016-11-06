/* future_alloc.c - future_alloc */

#include <xinu.h>
#include<future.h>
/*------------------------------------------------------------------------
 *  future_alloc-allocates a new future in given mode
 *------------------------------------------------------------------------
 */

typedef struct futent future;

future* future_alloc(int future_flag){
	future* f=(future*)getmem(sizeof(future));
	int *temp;
	f->flag=future_flag;
	f->state=FUTURE_EMPTY;
	temp=(int*)getmem(sizeof(int));
	f->value=temp;
	*temp=10;
	
	if(future_flag == FUTURE_SHARED)
		queueinit(&f->get_queue);
	
	else if(future_flag == FUTURE_QUEUE)
	{
		queueinit(&f->get_queue);
		queueinit(&f->get_queue);
	}
	return f;
}