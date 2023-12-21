#include <stdio.h>
#include <signal.h>

// Signal handler function for SIGUSR1 and SIGUSR2
void SigUsrHandler(int signo)
{
    if (signo == SIGUSR1)  {
        printf("Received a SIGUSR1 signal\n");  // Print a message indicating the reception of SIGUSR1 signal
    }
    else if (signo == SIGUSR2)  {
        printf("Received a SIGUSR2 signal\n");  // Print a message indicating the reception of SIGUSR2 signal
    }
    else  {
        printf("Received unknown signal\n");  // Print a message indicating the reception of an unknown signal
        printf("Terminate this process\n");   // Print a message indicating the termination of the process
        exit(0);
    }
}


int main()
{
    // Register SigUsrHandler as the handler for the SIGUSR1 signal
    if (signal(SIGUSR1, SigUsrHandler) == SIG_ERR)  {
        perror("signal");  // Print an error message if registering the signal handler fails
        exit(1);
    }

    // Register SigUsrHandler as the handler for the SIGUSR2 signal
    if (signal(SIGUSR2, SigUsrHandler) == SIG_ERR)  {
        perror("signal");  // Print an error message if registering the signal handler fails
        exit(1);
    }

    for (;;)
        pause();  // Enter an infinite loop, pausing the program until a signal is received
}

