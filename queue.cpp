#include	"queue.h"

#define	DFL_ZQUEUE_SIZE			8192
#define	ZQUEUE_MAX_SIZE			256

static		queue_t * queues[ZQUEUE_MAX_SIZE];
unsigned 	short	   queue_id;

static int			_enqueue(queue_t *queue, void *data, int count);
static int			_dequeue(queue_t *queue, void *data, int count);
static queue_t *	_create_queue(int size);
static int			queue_index(int id);
static queue_t * 	get_queue(int id);
static int			_test_dequeue_data(queue_t * queue, void *data, int count);


#define	_queue_empty(queue) \
	(queue->front == queue->rear)

#define	_queue_full(queue)	\
	((queue->rear + 1) % queue->size == queue->front)

#define	_test_enqueue(queue, count)	\
	(count > 0 && count <= _enqueue_space(queue))

#define	_test_dequeue(queue, count)	\
	(count > 0 && count <= _dequeue_space(queue))

#define	_enqueue_space(queue)	\
	((queue->rear < queue->front) ? queue->front - queue->rear - 1 : \
									  queue->size - (queue->rear - queue->front) - 1)
									  
#define	_dequeue_space(queue)	\
	((queue->front <= queue->rear) ? queue->rear - queue->front : \
									   queue->size - (queue->front - queue->rear))
	
queue_t *
_create_queue(int size){
	int 	   index = 0;
	queue_t * queue = NULL;
	
	do {
		for (index = 0; index < ZQUEUE_MAX_SIZE; index++){
			if (queues[index] == NULL)
				break;
		}
		
		if (index == ZQUEUE_MAX_SIZE)
			break;
		
		queue = (queue_t *)malloc(sizeof(queue_t));
		if (queue == NULL)
			break;
		
		memset(queue, 0, sizeof(queue_t));
		
		queue->buf = (char *)malloc(size);
		if (queue->buf == NULL){
			free(queue);
			queue = NULL;
			break;
		}
		
		memset(queue->buf, 0, size);
		queues[index] = queue;
	} while(0);

	return queue;
}

int
create_dfl_queue(){
	return create_queue(DFL_ZQUEUE_SIZE);
}

int
create_queue(int size){
	queue_t * queue = NULL;
	
	if (size <= 0)
		size = DFL_ZQUEUE_SIZE;
		
	if ((queue = _create_queue(size))== NULL)
		return ERR_ZQUEUE_ID;
	
	queue->id 		= (int)queue_id++;
	queue->size	= size;
	
	pthread_mutex_init(&queue->q_mutex, NULL); 
	
	return queue->id;
}

int
destroy_queue(int id){
	int index = queue_index(id);
	
	if (index >= 0)
	{
		pthread_mutex_destroy(&queues[index]->q_mutex);
		
		free(queues[index]->buf);
		free(queues[index]);
		queues[index] = NULL;
		
		return 0;
	}
	else
		return -1;
}

int
init_queue(int id){
	queue_t * queue = NULL;
	
	if ((queue = get_queue(id)) != NULL){
		pthread_mutex_lock(&queue->q_mutex);
		
		queue->front = queue->rear = 0;
		memset(queue->buf, 0, queue->size);
		
		pthread_mutex_unlock(&queue->q_mutex);
	}
	
	return id;
}

int
enqueue(int id, void *data, int count){
	
	queue_t * queue = NULL;
	
	if (count <= 0 || (queue = get_queue(id)) == NULL)
		return -1;
	
//	pthread_cleanup_push(pthread_mutex_unlock, (void *)&queue->q_mutex);
	pthread_mutex_lock(&queue->q_mutex);
	
	count = _enqueue(queue, data, count);
	
	pthread_mutex_unlock(&queue->q_mutex);
//	pthread_cleanup_pop(0);
	
	return count;
}

int
dequeue(int id, void *data, int count){

	queue_t * queue = NULL;
	
	if (count <= 0 || (queue = get_queue(id)) == NULL)
		return -1;
	
//	pthread_cleanup_push(pthread_mutex_unlock, (void *)&queue->q_mutex);
	pthread_mutex_lock(&queue->q_mutex);
	
	count = _dequeue(queue, data, count);
	
	pthread_mutex_unlock(&queue->q_mutex);
//	pthread_cleanup_pop(0);
	
	return count;
}

int
_enqueue(queue_t *queue, void *data, int count){
	
	int		   remain = 0;
	
	if (_test_enqueue(queue, count)){
		if (queue->rear + count <= queue->size){
			memcpy(&queue->buf[queue->rear], data, count);
			
			if (queue->rear + count == queue->size)
				queue->rear = 0;
			else
				queue->rear += count;
		} else {
			memcpy(&queue->buf[queue->rear], data, queue->size - queue->rear);
			remain = (queue->rear + count) % queue->size;
			memcpy(&queue->buf[0], (char *)data + (queue->size - queue->rear), remain);
			queue->rear = remain;
		}
	} else {
		count = 0;
	}
	
	//if (count > 0)
	//	printf("enqueue %d\n", count);
	
	return count;
}

int
_dequeue(queue_t *queue, void *data, int count){
	
	int		   remain = 0;

	if (_test_dequeue(queue, count)){
		if (queue->front + count <= queue->size){
			memcpy(data, &queue->buf[queue->front], count);
			
			if (queue->front + count == queue->size)
				queue->front = 0;
			else
				queue->front += count;
		} else {
			memcpy(data, &queue->buf[queue->front], queue->size - queue->front);
			remain = (queue->front + count) % queue->size;
			memcpy((char *)data + (queue->size - queue->front), &queue->buf[0], remain);
			queue->front = remain;
		}
	} else if (_dequeue_space(queue)) {
//		printf("re-dequeue:%d\n", _dequeue_space(queue));
		count = _dequeue(queue, data, _dequeue_space(queue));
	} else {
		count = 0;
	}
	
	//if (count > 0)
	//	printf("dequeue %d\n", count);
	return count;
}

queue_t * 
get_queue(int id){
	
	int index = 0;
	
	if (id < 0)
		return NULL;
		
	for (index = 0; index < ZQUEUE_MAX_SIZE; index++){
		if (queues[index] != NULL && queues[index]->id == id)
			break;
	}
	
	return (index < ZQUEUE_MAX_SIZE ? queues[index] : NULL);
}

int
queue_index(int id)
{
	int index = 0;
	
	if (id < 0)
		return ERR_ZQUEUE_ID;
		
	for (index = 0; index < ZQUEUE_MAX_SIZE; index++){
		if (queues[index] != NULL && queues[index]->id == id)
			break;
	}
	
	if (index < ZQUEUE_MAX_SIZE)
		return index;
	else
		return ERR_ZQUEUE_ID;
}

int
test_enqueue(int id, int count){
	queue_t * queue = NULL;
	
	if ((queue = get_queue(id)) == NULL)
		return -1;
	
	return _test_enqueue(queue, count);
}

int
test_dequeue(int id, int count){
	queue_t * queue = NULL;
	
	if ((queue = get_queue(id)) == NULL)
		return -1;
	
	return _test_dequeue(queue, count);
}

int
test_dequeue_data(int id, void *data, int count){
	queue_t * queue = NULL;
	
	if ((queue = get_queue(id)) == NULL){
		printf("queue %d not found!\n", id);
		return -1;
	}
	
	return _test_dequeue_data(queue, data, count);
}

int
_test_dequeue_data(queue_t * queue, void *data, int count){
	int		   remain = 0;
	
	if (_test_dequeue(queue, count)){
		if (queue->front + count <= queue->size){
			memcpy(data, &queue->buf[queue->front], count);
		} else {
			memcpy(data, &queue->buf[queue->front], queue->size - queue->front);
			remain = (queue->front + count) % queue->size;
			memcpy((char *)data + (queue->size - queue->front), &queue->buf[0], remain);
		}
	}
	else
		count = 0;
	
	return count;
}

int
queue_empty(int id){
	queue_t * queue = NULL;
	
	if ((queue = get_queue(id)) == NULL)
		return TRUE;
	
	return _queue_empty(queue);
}

int
queue_full(int id){
	queue_t * queue = NULL;
	
	if ((queue = get_queue(id)) == NULL)
		return FALSE;
	
	return _queue_full(queue);
}

int
queue_size(int id){
	queue_t * queue = NULL;
	int size = 0;
	
	if ((queue = get_queue(id)) == NULL)
		size = 0;
	else
		size = queue->size;
		
	return size;
}

int
enqueue_space(int id){
	queue_t * queue = NULL;
	if ((queue = get_queue(id)) == NULL)
		return 0;
		
	return _enqueue_space(queue);
}

int
dequeue_space(int id){
	queue_t * queue = NULL;
	if ((queue = get_queue(id)) == NULL)
		return 0;
	
	return _dequeue_space(queue);
}

int
queue_front(int id) {
	queue_t * queue = NULL;
	if ((queue = get_queue(id)) == NULL)
		return 0;
	
	return queue->front;
}

int
queue_rear(int id) {
	queue_t * queue = NULL;
	if ((queue = get_queue(id)) == NULL)
		return 0;
	
	return queue->rear;
}