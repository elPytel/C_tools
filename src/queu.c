/*
 * By elPytel
 */

#include <stdlib.h>
#include "queue.h"

// create empty queue
queue_t* create_queue() {
	queue_t* q = (queue_t*)malloc(sizeof(queue_t));
	q->queue_head = 0;
	q->queue_tail = 0;
	return q;
}

// free queue
void free_queue(queue_t* q) {
    if (q == NULL) {
        return;
    }
	free(q);
	q = NULL;
}

// empty queue
int is_empty(queue_t* q) {
	return q->queue_head == q->queue_tail;
}

// add message to queue
void add_to_queue(queue_t* q, void* msg) {
	q->queue[q->queue_tail] = msg;
	q->queue_tail = (q->queue_tail + 1) % QUEUE_SIZE;
}

// get message from queue
void* get_from_queue(queue_t* q) {
	if (is_empty(q)) {
		return NULL;
	}
	void* msg = q->queue[q->queue_head];
	q->queue_head = (q->queue_head + 1) % QUEUE_SIZE;
	return msg;
}

// END OF FILE