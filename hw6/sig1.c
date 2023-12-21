#include <stdio.h>
#include <signal.h>

// Signal handler function for SIGINT
void SigIntHandler(int signo)
{
    printf("Received a SIGINT signal\n");   // Print a message indicating the reception of SIGINT signal
    printf("Terminate this process\n");      // Print a message indicating the termination of the process

    exit(0);  // Terminate the process with a status code of 0
}

// Main function
int main()
{
    signal(SIGINT, SigIntHandler);  // Register SigIntHandler as the handler for the SIGINT signal

    printf("Press ^C to quit\n");  // Prompt the user to press Ctrl+C to quit

    for (;;)
        pause();  // Enter an infinite loop, pausing the program until a signal is received
}

