/*Assignment 4*/

#include <xinu.h>
#include <prodcons.h>
#include<stdlib.h>
#include<future.h>

/*-----------------------------------------------------------------------------------------------------------------------------------
 * xsh_prodcons- This file will contain the code pertaining to prodcons command
 *------------------------------------------------------------------------------------------------------------------------------------
 */

int n; 

shellcmd xsh_prodcons(int nargs, char *args[])
{
	
  future *f1, *f2, *f3;
 
  f1 = future_alloc(FUTURE_EXCLUSIVE);
  f2 = future_alloc(FUTURE_EXCLUSIVE);
  f3 = future_alloc(FUTURE_EXCLUSIVE);
 
  resume( create(future_cons, 1024, 20, "fcons1", 1, f1) );
  resume( create(future_prod, 1024, 20, "fprod1", 1, f1) );
  resume( create(future_cons, 1024, 20, "fcons2", 1, f2) );
  resume( create(future_prod, 1024, 20, "fprod2", 1, f2) );
  resume( create(future_cons, 1024, 20, "fcons3", 1, f3) );
  resume( create(future_prod, 1024, 20, "fprod3", 1, f3) );
 
  return (0);
}
