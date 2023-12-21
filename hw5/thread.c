#include <stdio.h>
#include <pthread.h>


void
PrintMsg(char *msg) /* a function that print the string parameter and exit the pthread. */
{
	printf("%s", msg);

	pthread_exit(NULL); /* terminate a thread. the parameter means retrival value */
}

main()
{
	pthread_t	tid1, tid2;
	char		*msg1 = "Hello, ";
	char		*msg2 = "World!\n";

	if (pthread_create(&tid1, NULL, (void *)PrintMsg, (void *)msg1) < 0)  { /* create a thread. thread id is stored at tid, and second parameter means thread's attribute. the third paramter is what logic the thread will start from. the last paramter is the arguments that deliverd to the start logic. return 0 if OK, nonzero on error */
		perror("pthread_create");
		exit(1);
	}

        if (pthread_create(&tid2, NULL, (void *)PrintMsg, (void *)msg2) < 0)  { /* create another thread */
                perror("pthread_create");
                exit(1);
        }

	printf("Threads created: tid=%d, %d\n", tid1, tid2); /* print the thread id */

	if (pthread_join(tid1, NULL) < 0)  { /* wait for termination of another thread. the first paramenter is the thread id, and the second parameter means the pointer of return value of thread. return 0 if OK, nonzero on error */
		perror("pthread_join");
		exit(1);
	}
	if (pthread_join(tid2, NULL) < 0)  { /* wait for thread2 */
		perror("pthread_join");
		exit(1);
	}

	printf("Threads terminated: tid=%d, %d\n", tid1, tid2); 
}
