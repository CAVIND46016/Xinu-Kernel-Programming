/*Global variable for producer consumer*/
extern int n;
 /*this is just declaration*/
#include<future.h>
/*function Prototype*/
void producer(int);
void consumer(int);
uint future_cons(future *fut);
uint future_prod(future *fut);

/* Semaphore Declarations */
extern sid32 produced, consumed,sem1 ;