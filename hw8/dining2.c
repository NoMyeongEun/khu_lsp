#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/time.h>

#define	NUM_MEN		5
#define	NLOOPS		5

#define	LEFT(i)		((i+NUM_MEN-1)%NUM_MEN)
#define	RIGHT(i)	((i+1)%NUM_MEN)

enum  { THINKING, EATING, HUNGRY };

sem_t	Philosopher[NUM_MEN];
sem_t	Mutex;
int		State[NUM_MEN];

void
ThreadUsleep(int usecs)
{
	pthread_cond_t		cond;
	pthread_mutex_t		mutex;
	struct timespec		ts;
	struct timeval		tv;

	if (pthread_cond_init(&cond, NULL) < 0)  {
		perror("pthread_cond_init");
		pthread_exit(NULL);
	}
	if (pthread_mutex_init(&mutex, NULL) < 0)  {
		perror("pthread_mutex_init");
		pthread_exit(NULL);
	}

	gettimeofday(&tv, NULL);
	ts.tv_sec = tv.tv_sec + usecs/1000000;
	ts.tv_nsec = (tv.tv_usec + (usecs%1000000)) * 1000;
	if (ts.tv_nsec >= 1000000000)  {
		ts.tv_nsec -= 1000000000;
		ts.tv_sec++;
	}

	if (pthread_mutex_lock(&mutex) < 0)  {
		perror("pthread_mutex_lock");
		pthread_exit(NULL);
	}
	if (pthread_cond_timedwait(&cond, &mutex, &ts) < 0)  {
		perror("pthread_cond_timedwait");
		pthread_exit(NULL);
	}

	if (pthread_cond_destroy(&cond) < 0)  {
		perror("pthread_cond_destroy");
		pthread_exit(NULL);
	}
	if (pthread_mutex_destroy(&mutex) < 0)  {
		perror("pthread_mutex_destroy");
		pthread_exit(NULL);
	}
}

void
Thinking(int id)
{
	printf("Philosopher%d: Thinking.....\n", id);
	ThreadUsleep((rand()%200)*10000);
	printf("Philosopher%d: Want to eat.....\n", id);
}

void
Eating(int id)
{
	printf("Philosopher%d: Eating.....\n", id);
	ThreadUsleep((rand()%100)*10000);
}

void
Test(int id)
{
    /* If I'm HUNGRY and My left philsopher is NOT EATING and My right philospher is NOT EATING, then I can EAT */
	if (State[id] == HUNGRY && State[LEFT(id)] != EATING 
			&& State[RIGHT(id)] != EATING)  {
		State[id] = EATING;

        /* signal(s[i])*/
		if (sem_post(&Philosopher[id]) < 0)  { /* increase my philosopher semaphore value - I CAN EAT! */
			perror("sem_post");
			pthread_exit(NULL);
		}
	}
}

void
PickUp(int id)
{
    /* wait(mutex) */
	if (sem_wait(&Mutex) < 0)  { /* wait until No philosopher is in CS */
		perror("sem_wait");
		pthread_exit(NULL);
	}

	State[id] = HUNGRY;

	Test(id); /* test If I CAN EAT */

    /* signal(mutex) */
	if (sem_post(&Mutex) < 0)  { /* Get of of CS */
		perror("sem_post");
		pthread_exit(NULL);
	}
    /* wait(s[i]) */
	if (sem_wait(&Philosopher[id]) < 0)  { /* wait until my semaphore value is not zero - when I CAN EAT */
		perror("sem_wait");
		pthread_exit(NULL);
	}
}

void
PutDown(int id)
{
    /* wait(mutex) */
	if (sem_wait(&Mutex) < 0)  { /* wait if the other is in CS*/
		perror("sem_wait");
		pthread_exit(NULL);
	}

	State[id] = THINKING;

	Test(LEFT(id)); /* Since my eating is done, test IF my LEFT philosopher can eat */
	Test(RIGHT(id)); /* Since my eating is done, test IF my RIGHT philosopher can eat */

    /* signal(mutex) */
	if (sem_post(&Mutex) < 0)  { /* get out of CS */
		perror("sem_post");
		pthread_exit(NULL);
	}
}

/* Dining Philosopher : Deadlock-free version */
void
DiningPhilosopher(int *pId)
{
	int		i;
	int		id = *pId;

	for (i = 0 ; i < NLOOPS ; i++)  {

		Thinking(id);

		PickUp(id);

		Eating(id);

		PutDown(id);
	}

	printf("Philosopher%d: thinking & eating %d times.....\n", id, i);

	pthread_exit(NULL);
}

main()
{
	pthread_t	tid[NUM_MEN];
	int			i, id[NUM_MEN];

	srand(0x8888);

    /* make NUM_MEN binary semaphores for WHO CAN EAT - if the semaphore value is 1, then he has 2 chopsticks */
	for (i = 0 ; i < NUM_MEN ; i++)  {
		if (sem_init(&Philosopher[i], 0, 0) < 0)  { /* At first no one is eating, initalize to 0 */
			perror("sem_init");
			exit(1);
		}
		id[i] = i;
	}
    /* make a binary semaphore to protect CS. At first no one in CS, initialize to 1 */
	if (sem_init(&Mutex, 0, 1) < 0)  {
		perror("sem_init");
		exit(1);
	}
    /* make threads as the number of philosophers */
	for (i = 0 ; i < NUM_MEN ; i++)  {
		if (pthread_create(&tid[i], NULL, (void *)DiningPhilosopher, 
				(void *)&id[i]) < 0)  {
			perror("pthread_create");
			exit(1);
		}
	}

	for (i = 0 ; i < NUM_MEN ; i++)  {
		if (pthread_join(tid[i], NULL) < 0)  { /* wait until all threads exit*/
			perror("pthread_join");
			exit(1);
		}
	}

	for (i = 0 ; i < NUM_MEN ; i++)  {
		if (sem_destroy(&Philosopher[i]) < 0)  {  /* remove the philosopher semaphores */
			perror("sem_destroy");
		}
	}
}