// ***************************************************************************************
//    Project: Understanding Producer-Consumer and Publish-Subscribe Patterns in C
//    File: publisher_subscriber.c
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

#define QUEUE_SIZE 10

typedef struct
{
    int messages[QUEUE_SIZE];
    int front, rear, count;
    DThreadMutex mutex;
    DThreadCond cond;
} MessageQueue;

MessageQueue queue = {.front = 0, .rear = 0, .count = 0};

dthread_define_routine(publisher)
{
    for (int i = 1; i <= 20; i++)
    {
        dthread_mutex_lock(&queue.mutex);

        while (queue.count == QUEUE_SIZE)
        {
            dthread_cond_wait(&queue.cond, &queue.mutex);
        }

        queue.messages[queue.rear] = i;
        queue.rear = (queue.rear + 1) % QUEUE_SIZE;
        queue.count++;
        printf("Published: %d\n", i);

        dthread_cond_broadcast(&queue.cond);
        dthread_mutex_unlock(&queue.mutex);

        xsleep(1000);
    }
    return NULL;
}

dthread_define_routine(subscriber)
{
    int id = *((int*)data);
    for (int i = 1; i <= 10; i++)
    {
        dthread_mutex_lock(&queue.mutex);

        while (queue.count == 0)
        {
            dthread_cond_wait(&queue.cond, &queue.mutex);
        }

        int message = queue.messages[queue.front];
        queue.front = (queue.front + 1) % QUEUE_SIZE;
        queue.count--;
        printf("Subscriber %d received: %d\n", id, message);

        dthread_cond_broadcast(&queue.cond);
        dthread_mutex_unlock(&queue.mutex);

        xsleep(1500);
    }
    return NULL;
}

int main()
{
    DThread publisher_thread;
    DThread subscriber_threads[2];

    int subscriber_ids[2] = {1, 2};

    dthread_mutex_init(&queue.mutex, NULL);
    dthread_cond_init(&queue.cond, NULL);

    publisher_thread = dthread_init_thread(publisher, NULL);
    if (dthread_create(&publisher_thread, NULL) != 0)
    {
        fprintf(stderr, "Creating thread failed\n");
        return 1;
    }

    for (int i = 0; i < 2; i++)
    {
        subscriber_threads[i] = dthread_init_thread(subscriber, &subscriber_ids[i]);
        if (dthread_create(&subscriber_threads[i], NULL) != 0)
        {
            fprintf(stderr, "Creating thread failed\n");
            return 1;
        }
    }

    if (dthread_join(&publisher_thread) != 0)
    {
        fprintf(stderr, "Joining thread failed\n");
        return 1;
    }

    for (int i = 0; i < 2; i++)
    {
        if (dthread_join(&subscriber_threads[i]) != 0)
        {
            fprintf(stderr, "Joining thread failed\n");
            return 1;
        }
    }

    dthread_mutex_destroy(&queue.mutex);
    dthread_cond_destroy(&queue.cond);

    return 0;
}
