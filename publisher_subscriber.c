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

#include "common.h"

#define QUEUE_SIZE 10

typedef struct
{
    int messages[QUEUE_SIZE];
    int front, rear, count;
    CThreadsMutex mutex;
    CThreadsCond cond;
} MessageQueue;

MessageQueue queue = {.front = 0, .rear = 0, .count = 0};

// Publisher function
void* publisher(void* arg)
{
    for (int i = 1; i <= 20; i++)
    {
        cthreads_mutex_lock(&queue.mutex);

        while (queue.count == QUEUE_SIZE)
        {
            cthreads_cond_wait(&queue.cond, &queue.mutex);
        }

        queue.messages[queue.rear] = i;
        queue.rear = (queue.rear + 1) % QUEUE_SIZE;
        queue.count++;
        printf("Published: %d\n", i);

        cthreads_cond_broadcast(&queue.cond);
        cthreads_mutex_unlock(&queue.mutex);

        xsleep(1000);
    }
    return NULL;
}

// Subscriber function
void* subscriber(void* arg)
{
    int id = *((int*)arg);
    for (int i = 1; i <= 10; i++)
    {
        cthreads_mutex_lock(&queue.mutex);

        while (queue.count == 0)
        {
            cthreads_cond_wait(&queue.cond, &queue.mutex);
        }

        int message = queue.messages[queue.front];
        queue.front = (queue.front + 1) % QUEUE_SIZE;
        queue.count--;
        printf("Subscriber %d received: %d\n", id, message);

        cthreads_cond_broadcast(&queue.cond);
        cthreads_mutex_unlock(&queue.mutex);

        xsleep(1500);
    }
    return NULL;
}

int main()
{
    CThreadsThread publisher_thread;
    CThreadsThread subscriber_threads[2];
    CThreadsArgs publisher_thread_args, subscriber_threads_args[2];

    int subscriber_ids[2] = {1, 2};

    cthreads_mutex_init(&queue.mutex, NULL);
    cthreads_cond_init(&queue.cond, NULL);

    if (cthreads_thread_create(&publisher_thread, NULL, publisher, NULL, &publisher_thread_args) != 0)
    {
        fprintf(stderr, "Creating thread failed\n");
        return 1;
    }

    for (int i = 0; i < 2; i++)
    {
        if (cthreads_thread_create(&subscriber_threads[i], NULL, subscriber, &subscriber_ids[i], &subscriber_threads_args[i]) != 0)
        {
            fprintf(stderr, "Creating thread failed\n");
            return 1;
        }
    }

    if (cthreads_thread_join(publisher_thread, NULL) != 0)
    {
        fprintf(stderr, "Joining thread failed\n");
        return 1;
    }

    for (int i = 0; i < 2; i++)
    {
        if (cthreads_thread_join(subscriber_threads[i], NULL) != 0)
        {
            fprintf(stderr, "Joining thread failed\n");
            return 1;
        }
    }

    cthreads_mutex_destroy(&queue.mutex);
    cthreads_cond_destroy(&queue.cond);

    return 0;
}
