#include <stdio.h>
#include <signal.h>
#include <pthread.h>

#define THREAD_MAIN
/*
#define THREAD_1
#define THREAD_2
*/

// Signal handler function for SIGINT
void SigIntHandler(int signo)
{
    printf("Received a SIGINT signal by thread %d\n", pthread_self());  // Print a message indicating the reception of SIGINT signal and the thread ID
    printf("Terminate this process\n");  // Print a message indicating the termination of the process

    exit(0);
}

// Thread function 1
void Thread1(void *dummy)
{
#ifdef THREAD_1
    signal(SIGINT, SigIntHandler);  // Register SigIntHandler as the handler for SIGINT if THREAD_1 is defined
#endif

    while (1)
        ;  // Infinite loop to keep the thread running
}

// Thread function 2
void Thread2(void *dummy)
{
#ifdef THREAD_2
    signal(SIGINT, SigIntHandler);  // Register SigIntHandler as the handler for SIGINT if THREAD_2 is defined
#endif

    while (1)
        ;  // Infinite loop to keep the thread running
}

// Main function
int main()
{
    pthread_t tid1, tid2;

    // Create thread 1
    if (pthread_create(&tid1, NULL, (void *)Thread1, NULL) < 0)  {
        perror("pthread_create");  // Print an error message if creating thread 1 fails
        exit(1);  // Terminate the process with a status code of 1
    }

    // Create thread 2
    if (pthread_create(&tid2, NULL, (void *)Thread2, NULL) < 0)  {
        perror("pthread_create");  // Print an error message if creating thread 2 fails
        exit(1);
    }

    printf("Create two threads: tid1=%d, tid2=%d\n", tid1, tid2);  // Print messages indicating the creation of threads
    printf("Main thread: tid=%d\n", pthread_self());  // Print the thread ID of the main thread

#ifdef THREAD_MAIN
    signal(SIGINT, SigIntHandler);  // Register SigIntHandler as the handler for SIGINT if THREAD_MAIN is defined
#endif

    printf("Press ^C to quit\n");  // Prompt the user to press Ctrl+C to quit

    for (;;)
        pause();  // Enter an infinite loop, pausing the program until a signal is received
}

