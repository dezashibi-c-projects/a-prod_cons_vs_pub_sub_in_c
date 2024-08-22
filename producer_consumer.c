// ***************************************************************************************
//    Project: Understanding Producer-Consumer and Publish-Subscribe Patterns in C
//    File: producer_consumer.c
//    Date: 2024-08-08
//    Author: Navid Dezashibi
//    Contact: navid@dezashibi.com
//    Website: https://www.dezashibi.com | https://github.com/dezashibi
//    License:
//     Please refer to the LICENSE file, repository or website for more information about
//     the licensing of this work. If you have any questions or concerns,
//     please feel free to contact me at the email address provided above.
// ***************************************************************************************
// *  Description: Read the readme file for more information
// ***************************************************************************************

#define DTHREAD_IMPL
#include "common.h"
#include "dthreads/dthread.h"

#define BUFFER_SIZE 10

// Buffer and related variables
int buffer[BUFFER_SIZE];
int count = 0; // Number of items in the buffer

// Mutex and condition variables
DThreadMutex mutex;
DThreadCond cond_producer;
DThreadCond cond_consumer;

dthread_define_routine(producer)
{
    for (int i = 1; i <= 20; i++)
    {
        // Produce an item (just a number in this case)
        int item = i;

        // Lock the mutex before accessing the buffer
        dthread_mutex_lock(&mutex);

        // Wait until the buffer has space
        while (count == BUFFER_SIZE)
        {
            dthread_cond_wait(&cond_producer, &mutex);
        }

        // Add the item to the buffer
        buffer[count++] = item;
        printf("Produced: %d\n", item);

        // Signal the consumer that there are items in the buffer
        dthread_cond_signal(&cond_consumer);

        // Unlock the mutex
        dthread_mutex_unlock(&mutex);

        // Simulate some delay
        xsleep(1000);
    }
    return NULL;
}

dthread_define_routine(consumer)
{
    for (int i = 1; i <= 20; i++)
    {
        // Lock the mutex before accessing the buffer
        dthread_mutex_lock(&mutex);

        // Wait until there are items in the buffer
        while (count == 0)
        {
            dthread_cond_wait(&cond_consumer, &mutex);
        }

        // Remove the item from the buffer
        int item = buffer[--count];
        printf("Consumed: %d\n", item);

        // Signal the producer that there is space in the buffer
        dthread_cond_signal(&cond_producer);

        // Unlock the mutex
        dthread_mutex_unlock(&mutex);

        // Simulate some delay
        xsleep(1500);
    }
    return NULL;
}

int main(void)
{
    DThread producer_thread, consumer_thread;

    producer_thread = dthread_init_thread(producer, NULL);
    consumer_thread = dthread_init_thread(consumer, NULL);

    dthread_mutex_init(&mutex, NULL);
    dthread_cond_init(&cond_producer, NULL);
    dthread_cond_init(&cond_consumer, NULL);

    // Create the producer and consumer threads
    if (dthread_create(&producer_thread, NULL) != 0)
    {
        fprintf(stderr, "Creating thread failed\n");
        return 1;
    }

    if (dthread_create(&consumer_thread, NULL) != 0)
    {
        fprintf(stderr, "Creating thread failed\n");
        return 1;
    }

    // Wait for both threads to finish
    if (dthread_join(&producer_thread) != 0)
    {
        fprintf(stderr, "Joining thread failed\n");
        return 1;
    }

    if (dthread_join(&consumer_thread) != 0)
    {
        fprintf(stderr, "Joining thread failed\n");
        return 1;
    }

    // Clean up
    dthread_mutex_destroy(&mutex);
    dthread_cond_destroy(&cond_producer);
    dthread_cond_destroy(&cond_consumer);

    return 0;
}
