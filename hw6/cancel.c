#include <stdio.h>
#include <signal.h>
#include <pthread.h>

pthread_t ThreadId[2];

// Signal handler function for SIGINT
void SigIntHandler(int signo)
{
    int i;

    printf("Received a SIGINT signal by thread %d\n", pthread_self());
    printf("Terminate other threads: tid=%d, %d\n", ThreadId[0], ThreadId[1]);

    // Cancel and join each thread
    for (i = 0; i < 2; i++) {
        if (pthread_cancel(ThreadId[i])) {
            perror("pthread_cancel");
            exit(1);
        }
    }
    for (i = 0; i < 2; i++) {
        if (pthread_join(ThreadId[i], NULL)) {
            perror("pthread_join");
            exit(1);
        }
    }

    printf("Threads terminated: tid=%d, %d\n", ThreadId[0], ThreadId[1]);

    exit(0);
}

// Thread function 1
void Thread1(void *dummy)
{
    printf("Thread %d created.....\n", pthread_self());

    // Set cancel state and type
    if (pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL)) {
        perror("pthread_setcancelstate");
        pthread_exit(NULL);
    }
    if (pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL)) {
        perror("pthread_setcanceltype");
        pthread_exit(NULL);
    }

    while (1)
        ;  // Infinite loop to keep the thread running
}

// Thread function 2
void Thread2(void *dummy)
{
    printf("Thread %d created.....\n", pthread_self());

    // Set cancel state and type
    if (pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL)) {
        perror("pthread_setcancelstate");
        pthread_exit(NULL);
    }
    if (pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL)) {
        perror("pthread_setcanceltype");
        pthread_exit(NULL);
    }

    while (1)
        ;  // Infinite loop to keep the thread running
}

// Main function
int main()
{
    // Create thread 1
    if (pthread_create(&ThreadId[0], NULL, (void *)Thread1, NULL) < 0) {
        perror("pthread_create");
        exit(1);
    }

    // Create thread 2
    if (pthread_create(&ThreadId[1], NULL, (void *)Thread2, NULL) < 0) {
        perror("pthread_create");
        exit(1);
    }

    signal(SIGINT, SigIntHandler);  // Register SigIntHandler as the handler for SIGINT

    printf("Press ^C to quit\n");  // Prompt the user to press Ctrl+C to quit

    for (;;)
        pause();  // Enter an infinite loop, pausing the program until a signal is received
}

