#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define BUFFER_SIZE 20
#define NUMITEMS 30

typedef struct {
	    int msg[BUFFER_SIZE];
	    int Finmsg;
	    int in, out;
	    pthread_mutex_t mutex;
	    pthread_cond_t full;
	    pthread_cond_t empty;
} buffer_t;

buffer_t bb = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 0, 0, 0,
	           PTHREAD_MUTEX_INITIALIZER,
	           PTHREAD_COND_INITIALIZER,
	           PTHREAD_COND_INITIALIZER};

int prod_msg() {
	    int msg = (int) (100.0 * rand() / (RAND_MAX + 1.0));
	    sleep((unsigned long) 2);
	    printf("--------------------------------\n");
	    printf("\n %d 테스트 방송 \n ", msg);

	    return msg;
}

int insert_msg(int msg) {
	    int status;
	    status = pthread_mutex_lock(&bb.mutex);
	    if (status != 0)
	        return status;
	    while (bb.Finmsg >= BUFFER_SIZE && status == '\0')
	        status = pthread_cond_wait(&bb.empty, &bb.mutex);
	    if (status != 0) {
	        pthread_mutex_unlock(&bb.mutex);
	        return status;
	    }
	    bb.msg[bb.in] = msg;
	    bb.in = (bb.in + 1) % BUFFER_SIZE;
	    bb.Finmsg++;
	    if (status = pthread_cond_signal(&bb.full)) {
	         pthread_mutex_unlock(&bb.mutex);
	         return status;
	    }
	    return pthread_mutex_unlock(&bb.mutex);
}

int cons_msg(int msg) {
	    sleep((unsigned long) 2);
	    printf("\n %d 테스트 방송 들음 \n", msg);
	    printf("--------------------------------\n");
}
int remove_msg(int *temp) {
	    int status;
	    status = pthread_mutex_lock(&bb.mutex);
	    if (status != 0)
	        return status;
	    while (bb.Finmsg <= 0 && status == '\0')
	        status = pthread_cond_wait(&bb.full, &bb.mutex);
	    if (status != 0) {
	        pthread_mutex_unlock(&bb.mutex);
	        return status;
	    }
	        *temp = bb.msg[bb.out];
	        bb.out = (bb.out + 1) % BUFFER_SIZE;
	        bb.Finmsg--;
	    if (status = pthread_cond_signal(&bb.empty)) {
	        pthread_mutex_unlock(&bb.mutex);
	        return status;
	    }
	    return pthread_mutex_unlock(&bb.mutex);
}

void *prod(void *arg) {
	    int msg;
	    while (1) {
	        msg = prod_msg();
	        insert_msg(msg);
	    }
}

void *cons(void *arg) {
	    int msg;
            while (1) {
	        remove_msg(&msg);
	        cons_msg(msg);
	    }
}

void main() {
	    int status;
            void *result;
	    pthread_t producer_tid, consumer_tid;

	    
	    status = pthread_create(&producer_tid, NULL, prod, NULL);
	    if (status != 0)
	          perror("Create producer thread");
	    status = pthread_create(&consumer_tid, NULL, cons, NULL);
	    if (status != 0)
	           perror("Create consumer thread");
	    status = pthread_join(producer_tid, NULL);
	    if (status != 0)
	           perror("Join producer thread");
	    status = pthread_join(consumer_tid, NULL);
	    if (status != 0)
	           perror("Join consumer thread");
}
