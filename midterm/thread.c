#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

int	matrix1[2][2];
int 	matrix2[2][2];

void
task(int idx, int **matrix)
{
	int first, second, third, fourth;

	if (idx == 1 || idx == 2) {
		first = matrix1[0][0];
		third = matrix1[0][1];
	}
	else 
	{
		first = matrix1[1][0];
		third = matrix1[1][1];
	}

	if (idx == 1 || idx == 3) 
	{
		second = matrix2[0][0];
		fourth = matrix2[1][0];
	}
	else 
	{
		second = matrix2[0][1];
		fourth = matrix2[1][1];
	}

	int res = first * second + third * fourth;
	pthread_exit(res); /* thread termination and return the parent process, the result of sum */
}

main()
{
	pthread_t	tid1, tid2, tid3, tid4;
	int		res1, res2, res3, res4;

	printf("(");
	int i, j;
	for (i = 0; i < 2; i++)
	{
		for (j = 0; j < 2; j++)
		{
			matrix1[i][j] = rand() % 50;
			printf("%d", matrix1[i][j]);
			if (i != 1 || j != 1) printf(" ");
		}
	}
	printf(")x(");

	for (i = 0; i < 2; i++)
	{
		for (j = 0; j < 2; j ++)
		{
			matrix2[i][j] = rand() % 50;
			printf("%d",matrix2[i][j]);
			if (i != 1 || j != 1) printf(" ");
		}
	}
	printf(")=");

	if (pthread_create(&tid1, NULL, (void *)task, (void *)1) < 0)  {
		perror("pthread_create");
		exit(1);
	}

	if (pthread_create(&tid2, NULL, (void *)task, (void *)2) < 0)  {
		perror("pthread_create");
		exit(1);
	}
	
	if (pthread_create(&tid3, NULL, (void *)task, (void *)3) < 0)  {
		perror("pthread_create");
		exit(1);
	}

	if (pthread_create(&tid4, NULL, (void *)task, (void *)4) < 0)  {
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

	if (pthread_join(tid3, &res3) < 0)  { /* wait for the thread1 termination */
		perror("pthread_join");
		exit(1);
	}
	if (pthread_join(tid4, &res4) < 0)  { /* wait for the thread2 termination */
		perror("pthread_join");
		exit(1);
	}

	printf("(%d %d %d %d)\n",res1, res2, res3, res4); /* the total sum is sum of return value of thread1 and thread2 */
}
