
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 3
#define NUM_PRODUCERS 2
#define NUM_CONSUMERS 2

typedef struct {
	  int buffer[BUFFER_SIZE];
	  int in;  
	  int out; 
	   pthread_mutex_t lock;
	   pthread_cond_t not_full;
	   pthread_cond_t not_empty;
} BoundedBuffer;

BoundedBuffer buffer;

void initBuffer(BoundedBuffer* buffer) {
	  buffer->in = 0;
	  buffer->out = 0;
	  pthread_mutex_init(&buffer->lock, NULL);
	  pthread_cond_init(&buffer->not_full, NULL);
	  pthread_cond_init(&buffer->not_empty, NULL);
}

void produce(BoundedBuffer* buffer, int item) {
	    pthread_mutex_lock(&buffer->lock);
	        
	    while ((buffer->in + 1) % BUFFER_SIZE == buffer->out) {
	        pthread_cond_wait(&buffer->not_full, &buffer->lock);
	    }

	    buffer->buffer[buffer->in] = item;
	    buffer->in = (buffer->in + 1) % BUFFER_SIZE;

	    printf("Produced %d. Buffer: [", item);
	    for (int i = 0; i < BUFFER_SIZE; ++i) {
		        printf("%d ", buffer->buffer[i]);
	    }
		    printf("]\n");

	    pthread_cond_signal(&buffer->not_empty);
	    pthread_mutex_unlock(&buffer->lock);
}

int consume(BoundedBuffer* buffer) {
	    pthread_mutex_lock(&buffer->lock);

	    while (buffer->in == buffer->out) {
	       pthread_cond_wait(&buffer->not_empty, &buffer->lock);
	    }

	    int item = buffer->buffer[buffer->out];
	    buffer->out = (buffer->out + 1) % BUFFER_SIZE;

	    printf("Consumed %d. Buffer: [", item);
	    for (int i = 0; i < BUFFER_SIZE; ++i) {
	        printf("%d ", buffer->buffer[i]);
	    }
	    printf("]\n");

	    pthread_cond_signal(&buffer->not_full);
	    pthread_mutex_unlock(&buffer->lock);

	    return item;
}

void* producer(void* arg) {
	    int producer_id = *(int*)arg;

	    for (int i = 0; i < 5; ++i) {
	        usleep(rand() % 1000000); 
		produce(&buffer, producer_id * 10 + i);
	    }

	    return NULL;
}

void* consumer(void* arg) {
	    int consumer_id = *(int*)arg;

	    for (int i = 0; i < 5; ++i) {
	        usleep(rand() % 1000000);
	        consume(&buffer);
	    }

	    return NULL;
}

int main() {
	    initBuffer(&buffer);

	     pthread_t producers[NUM_PRODUCERS];
	     pthread_t consumers[NUM_CONSUMERS];

	     int producer_ids[NUM_PRODUCERS];
	     int consumer_ids[NUM_CONSUMERS];

	     for (int i = 0; i < NUM_PRODUCERS; ++i) {
		        producer_ids[i] = i;
		        pthread_create(&producers[i], NULL, producer, &producer_ids[i]);
	    }

	     for (int i = 0; i < NUM_CONSUMERS; ++i) {
  	                consumer_ids[i] = i;
	                pthread_create(&consumers[i], NULL, consumer, &consumer_ids[i]);
	     }

	     for (int i = 0; i < NUM_PRODUCERS; ++i) {
		        pthread_join(producers[i], NULL);
	     }

	     for (int i = 0; i < NUM_CONSUMERS; ++i) {
	                pthread_join(consumers[i], NULL);
	     }

	       return 0;
}

