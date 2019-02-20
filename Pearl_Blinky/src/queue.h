/*
 * queue.h
 *
 *  Created on: Feb 5, 2019
 *      Author: Jacob S
 */

#ifndef SRC_QUEUE_H_
#define SRC_QUEUE_H_

#include "main.h"
#include <stdbool.h>

struct Transmit_Queue{
	uint16_t queue[10];
	uint8_t maxSize;
	uint8_t numItems;
	uint8_t front;
	uint8_t end;
};

bool isEmpty(struct Transmit_Queue* t_queue);						//Returns true if empty, false if not empty

bool isFull(struct Transmit_Queue* t_queue);						//Returns true if full, false if not full

bool enqueue(struct Transmit_Queue* t_queue, uint16_t item);			//add item to transmit queue

bool dequeue(struct Transmit_Queue* t_queue);					//service item from the transmit queue

void clear_queue(struct Transmit_Queue* t_queue);					//Clear all items in the queue

void initialize_queue(struct Transmit_Queue* t_queue);

#endif /* SRC_QUEUE_H_ */
