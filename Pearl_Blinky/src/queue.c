/*
 * queue.c
 *
 *  Created on: Feb 5, 2019
 *      Author: Jacob S
 */
#include "queue.h"

bool isEmpty(struct Transmit_Queue* t_queue)
{
	if(t_queue->numItems == 0)
	{
		return true;
	}
	return false;
}

bool isFull(struct Transmit_Queue* t_queue)
{
	if(t_queue->numItems == t_queue->maxSize)
	{
		return true;
	}
	return false;
}

bool enqueue(struct Transmit_Queue* t_queue, uint16_t item)
{
	if(isFull(t_queue))
	{
		return false;
	}
	else if(isEmpty(t_queue))
	{
		t_queue->front++;
		t_queue->end++;
		t_queue->queue[t_queue->end] = item;
	}
	else if(t_queue->end == (t_queue->maxSize - 1))
	{
		t_queue->end = 0;
		t_queue->queue[t_queue->end] = item;
	}
	else
	{
		t_queue->end++;
		t_queue->queue[t_queue->end] = item;
	}
	t_queue->numItems++;
	return true;
}

bool dequeue(struct Transmit_Queue* t_queue){
	if(isEmpty(t_queue))
	{
		return false;
	}
	else if(t_queue->end == t_queue->front)
	{
		t_queue->end = -1;
		t_queue->front = -1;
	}
	else if(t_queue->front == (t_queue->maxSize -1))
	{
		t_queue->front = 0;
	}
	else
	{
		t_queue->front++;
	}
	t_queue->numItems--;
	return true;
}

void clear_queue(struct Transmit_Queue* t_queue)
{
	t_queue->numItems = 0;
	t_queue->front = -1;
	t_queue->end = -1;
}

void initialize_queue(struct Transmit_Queue* t_queue)
{
	t_queue->front = -1;
	t_queue->end = -1;
	t_queue->numItems = 0;
	t_queue->maxSize = 10;
}
