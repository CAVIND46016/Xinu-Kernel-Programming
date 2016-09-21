#include <xinu.h>
//#include <prodcons.h>

void producer(int count) {
	//Code to produce values less than equal to count, 
    	//produced value should get assigned to global variable 'n'.
    	//print produced value e.g. produced : 8

	int i;
	
	for(i = 0;i < count;i++)
	{
		n++;
	}

	printf("Goods produced: %d \n", n);
}



