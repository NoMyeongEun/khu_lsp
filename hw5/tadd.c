#include <stdio.h>
#include <pthread.h>

void
task(int arg) /* a function that thead will do. the argument is thread index */
{
	int	res = 0; /* the return value initialization */
	int	start = 50 * arg; /* start index according to the thread index */
	int	end = start + 50; /* end index according to the start index */
	for (int i = start + 1; i <= end; i++) /* add the number of a given range */ 
		res += i;
	pthread_exit(res); /* thread termination and return the parent process, the result of sum */
}

main()
{
	pthread_t	tid1, tid2;
	int		res1, res2;

	if (pthread_create(&tid1, NULL, (void *)task, (void *)0) < 0)  { /* create a thread that will add 1 ~ 50. the thread idx is 0 */
		perror("pthread_create");
		exit(1);
	}

	if (pthread_create(&tid2, NULL, (void *)task, (void *)1) < 0)  { /* create a thread that will add 51 ~ 100. the thread idx is 1 */
		perror("pthread_create");
		exit(1);
	}
	
	if (pthread_join(tid1, &res1) < 0)  { /* wait for the thread1 termination */
		perror("pthread_join");
		exit(1);
	}
	if (pthread_join(tid2, &res2) < 0)  { /* wait for the thread2 termination */
		perror("pthread_join");
		exit(1);
	}

	printf("sum = %d\n",res1 + res2); /* the total sum is sum of return value of thread1 and thread2 */
}
