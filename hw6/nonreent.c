#include <stdio.h>
#include <signal.h>
#include <pwd.h>

// Signal handler function for SIGALRM
void MyAlarmHandler(int signo)
{
    struct passwd *rootptr;

    signal(SIGALRM, MyAlarmHandler);  // Re-register MyAlarmHandler as the handler for the SIGALRM signal
    alarm(1);  // Set a periodic alarm to wake up the program after 1 second

    printf("in signal handler\n");  // Print a message indicating entry into the signal handler

    // Attempt to retrieve information about the root user
    if ((rootptr = getpwnam("root")) == NULL)  {
        perror("getpwnam");  // Print an error message if getpwnam fails
        exit(1);
    }

    return;
}

// Main function
int main()
{
    struct passwd *ptr;

    signal(SIGALRM, MyAlarmHandler);  // Register MyAlarmHandler as the handler for the SIGALRM signal
    alarm(1);  // Set a periodic alarm to wake up the program after 1 second

    for (;;)  {
        // Attempt to retrieve information about the user "cjs"
        if ((ptr = getpwnam("cjs")) == NULL)  {
            perror("getpwnam");  // Print an error message if getpwnam fails
            exit(1);
        }

        // Check if the return value is corrupted
        if (strcmp(ptr->pw_name, "cjs") != 0)  {
            printf("return value corrupted!, pw_name = %s\n", ptr->pw_name);  // Print a message indicating a corrupted return value
            exit(0);
        }
    }
}

