#include<xinu.h>
#include<queuem.h>

typedef struct queuestruct queue;

int queueinit(queue *queue)
{	
	queue->data = (int *)getmem(QUEUELEN);
	queue->front = 0;
	queue->rear = -1;
	queue->count = 0;
}
	

int qinsert(queue *temp, int tdata)
{
	if(temp->rear+1 > QUEUELEN)
		return SYSERR;
	temp->data[++temp->rear] = tdata;
	temp->count++;
	return OK;
}

int qdelete(queue *temp)
{
	if(temp->count < 1)
		return SYSERR;
	temp->count--;
	return (temp->data[temp->front++]);   
}