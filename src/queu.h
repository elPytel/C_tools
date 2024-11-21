/*
 * By elPytel
 */

// generic queue of void pointers
#define QUEUE_SIZE 10

// queue struct
typedef struct {
	void* queue[QUEUE_SIZE];
	int queue_head;
	int queue_tail;
} queue_t;

// queue functions:

// create empty queue
queue_t* create_queue();

// free queue
void free_queue(queue_t* q);

// empty queue
int is_empty(queue_t* q);

// add message to queue
void add_to_queue(queue_t* q, void* msg);

// get message from queue
void* get_from_queue(queue_t* q);
