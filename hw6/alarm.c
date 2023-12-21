#include <stdio.h>
#include <signal.h>

static unsigned int AlarmSecs;  // Variable to store the alarm duration

// Signal handler function for SIGALRM
void SigAlarmHandler(int signo)
{
    // Register SigAlarmHandler as the handler for the SIGALRM signal
    if (signal(SIGALRM, SigAlarmHandler) == SIG_ERR)  {
        perror("signal");  // Print an error message if registering the signal handler fails
        exit(1);  // Terminate the process with a status code of 1
    }

    alarm(AlarmSecs);  // Set a periodic alarm to wake up the program after the specified time

    /* Do something */
    printf(".");  // Print a dot to indicate that something is happening
    fflush(stdout);  // Flush the output buffer to ensure the dot is immediately visible

    return;
}

// Function to set a periodic alarm
int SetPeriodicAlarm(unsigned int nsecs)
{
    // Register SigAlarmHandler as the handler for the SIGALRM signal
    if (signal(SIGALRM, SigAlarmHandler) == SIG_ERR)  {
        return -1;  // Return -1 if registering the signal handler fails
    }

    AlarmSecs = nsecs;  // Set the global variable AlarmSecs to the specified time

    alarm(nsecs);  // Set the initial alarm to wake up the program after the specified time

    return 0;  // Return 0 to indicate success
}

// Main function
int main()
{
    printf("Doing something every one second\n");

    SetPeriodicAlarm(1);  // Call the function to set a periodic alarm for every one second

    for (;;)
        pause();  // Enter an infinite loop, pausing the program until a signal is received
}

