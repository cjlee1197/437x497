#ifndef		__QUEUE_H
#define		__QUEUE_H

#include	"common.h"

struct z_queue {
	int 	id;			/* queue id */
	int 	front;		/* queue front position */
	int 	rear;		/* queue rear position */
	int 	size;		/* queue size */
	char 	*buf;		/* queue buffer */
	pthread_mutex_t q_mutex;	/* internal operation lock */
};

typedef	struct z_queue queue_t;

#define	ERR_ZQUEUE_ID	-1
									
int	create_queue(int size);
int create_dfl_queue();
int	destroy_queue(int id);
int init_queue(int id);

int	enqueue(int id, void *data, int count);
int	dequeue(int id, void *data, int count);

int	test_enqueue(int id, int count);
int	test_dequeue(int id, int count);
int	test_dequeue_data(int id, void *data, int count);

int	queue_empty(int id);
int	queue_full(int id);
int queue_size(int id);

int	enqueue_space(int id);
int dequeue_space(int id);

int queue_front(int id);
int	queue_rear(int id);

#endif
