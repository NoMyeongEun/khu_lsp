#include <stdio.h>
#include <signal.h>

void SigAlarmHandler(int signo)
{
    /* nothing to do, just return to wake up the pause */
    return;
}

unsigned int mysleep(unsigned int nsecs)
{
    // Register SigAlarmHandler as the handler for the SIGALRM signal
    if (signal(SIGALRM, SigAlarmHandler) == SIG_ERR)  {
        return nsecs;  // Return the remaining sleep time if registering the signal handler fails
    }

    alarm(nsecs);  // Set an alarm to wake up the program after the specified time

    pause();  // Pause the program until a signal is received

    return alarm(0);  // Return the remaining time on the alarm, which is the time left to sleep
}

int main()
{
    printf("Wait for 5 seconds...\n");

    mysleep(5);  // Call custom sleep function to wait for 5 seconds
}

