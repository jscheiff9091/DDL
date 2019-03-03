/*
 * queue.c
 *
 *  Created on: Feb 5, 2019
 *      Author: Jacob S
 */
#include "queue.h"

bool isEmpty(Receive_Buffer_t * r_buff)
{
	if(r_buff->numItems == 0)
	{
		return true;
	}
	return false;
}

bool isFull(Receive_Buffer_t * r_buff)
{
	if(r_buff->numItems == r_buff->maxSize)
	{
		bool success = extend_buffer(r_buff);

		if(success) {
			return false;
		}
		else {
			return true;
		}
	}
	return false;
}

bool enqueue(Receive_Buffer_t * r_buff, uint8_t item)
{
	if(isFull(r_buff))
	{
		return false;
	}
	else if(isEmpty(r_buff))
	{
		r_buff->front++;
		r_buff->end++;
		r_buff->queue[r_buff->end] = item;
	}
	else if(r_buff->end == (r_buff->maxSize - 1))
	{
		r_buff->end = 0;
		r_buff->queue[r_buff->end] = item;
	}
	else
	{
		r_buff->end++;
		r_buff->queue[r_buff->end] = item;
	}
	r_buff->numItems++;
	return true;
}

bool dequeue(Receive_Buffer_t * r_buff){
	if(isEmpty(r_buff))
	{
		return false;
	}
	else if(r_buff->end == r_buff->front)
	{
		r_buff->end = -1;
		r_buff->front = -1;
	}
	else if(r_buff->front == (r_buff->maxSize -1))
	{
		r_buff->front = 0;
	}
	else
	{
		r_buff->front++;
	}
	r_buff->numItems--;
	return true;
}

void clear_queue(Receive_Buffer_t * r_buff)
{
	r_buff->numItems = 0;
	r_buff->front = -1;
	r_buff->end = -1;
}

void initialize_queue(Receive_Buffer_t * r_buff)
{
	r_buff->front = -1;
	r_buff->end = -1;
	r_buff->numItems = 0;
	r_buff->maxSize = 25;
	r_buff->queue = (uint8_t *) malloc(sizeof(uint8_t) * r_buff->maxSize);
}

bool extend_buffer(Receive_Buffer_t * r_buff)
{
	if((r_buff->maxSize * 2) > MAX_BUFFER_SIZE)
	{
		return false;
	}
	else
	{
		uint8_t temp_array[r_buff->maxSize];
		uint8_t temp_array_size = r_buff->maxSize;
		copy_array_data(temp_array, r_buff->queue, temp_array_size, r_buff->maxSize);

		free(r_buff->queue);
		r_buff->maxSize = r_buff->maxSize * 2;
		r_buff->queue = (uint8_t *) malloc(sizeof(uint8_t) * r_buff->maxSize);

		copy_array_data(r_buff->queue, temp_array, r_buff->maxSize, temp_array_size);
	}
	return true;
}

void copy_array_data(uint8_t* new_array, uint8_t* old_array, uint8_t size_of_new, uint8_t size_of_old)
{
	int i = 0;

	while(i < size_of_old)
	{
		new_array[i] = old_array[i];
		i++;
	}

	if(size_of_new > size_of_old)
	{
		while(i < size_of_new)
		{
			new_array[i] = 0;
			i++;
		}
	}
}
