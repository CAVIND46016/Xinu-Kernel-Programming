#ifndef _QUEUEM_H_
#define _QUEUEM_H_

#define QUEUELEN 100

typedef struct queuestruct{
	int *data;
	int front;
	int rear;
	int count
} queue ;

int queue_init(queue*);

int qinsert(queue*,int);

int qdelete(queue*);

#endif 