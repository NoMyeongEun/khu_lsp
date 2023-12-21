#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

/* POSIX semaphore-like library using System V semaphore */
int		semInit(key_t key);
int		semInitValue(int semid, int value);
int		semWait(int semid);
int		semTryWait(int semid);
int		semPost(int semid);
int		semGetValue(int semid);
int		semDestroy(int semid);