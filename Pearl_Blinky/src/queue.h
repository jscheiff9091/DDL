/*
 * queue.h
 *
 *  Created on: Feb 5, 2019
 *      Author: Jacob S
 */

#ifndef SRC_QUEUE_H_
#define SRC_QUEUE_H_

#include "main.h"

#define MAX_BUFFER_SIZE 	100

typedef struct {
	uint8_t * queue;
	uint8_t maxSize;
	uint8_t numItems;
	int8_t front;
	int8_t end;
}Receive_Buffer_t;

bool isEmpty(Receive_Buffer_t * r_buff);						//Returns true if empty, false if not empty

bool isFull(Receive_Buffer_t * r_buff);						//Returns true if full, false if not full

bool enqueue(Receive_Buffer_t * r_buff, uint8_t item);			//add item to transmit queue

bool dequeue(Receive_Buffer_t * r_buff);					//service item from the transmit queue

void clear_queue(Receive_Buffer_t * r_buff);					//Clear all items in the queue

void initialize_queue(Receive_Buffer_t * r_buff);

bool extend_buffer(Receive_Buffer_t * r_buff);

void copy_array_data(uint8_t* new_array, uint8_t* old_array, uint8_t size_of_new, uint8_t size_of_old);

#endif /* SRC_QUEUE_H_ */
