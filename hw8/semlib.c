#include <stdio.h>
#include "semlib.h"
 
 
int
semInit(key_t key) /* make a semaphore using key (if already exists, then return ID )*/
{
    int     semid;

    if ((semid = semget(key, 1, 0600 | IPC_CREAT)) < 0)  { 
        /* semget : obtain a semaphore set ID
        - named 'key'
        - number of semaphore is 1
        - flag is IPC_CREAT
        - return semaphore ID if OK, -1 on error
        */
		perror("semget");
        return -1;
    }

    return semid;
}
 
int
semInitValue(int semid, int value) /* Initialize semaphore value */
{
    union semun {
            int     val;
    } semun;

    semun.val = value;
    if (semctl(semid, 0, SETVAL, semun) < 0)  {
        /* semctl : semaphore control operations
        - int semid 
        - int semnum : number of semaphore = 1
        - int cmd : command -> IPC_STAT, IPC_SET, IPC_RMID, GETVAL, SETVAL, GETPID, GETNCNT, GETZCNT, GETALL, SETALL
        - union semun arg : arguments
            * int val : need for SETVAL
            * struct semid_ds *buf : need for IPC_STAT and IPC_SET
            * ushort *array : need for GETALL and SETALL
        - return non-negative value depending on cmd if OK, -1 on error
        */
		perror("semctl");
        return -1;
    }
 
    return semid;
}
 
int
semWait(int semid) /* decrease semaphore value or Wait */
{
    struct sembuf   semcmd;
 
    semcmd.sem_num = 0; /* ushort sem_num : member # in set (0, 1, ... , nsems -1)*/
    semcmd.sem_op = -1; /* short semp_op : operation (negative, 0, or positive)*/
    semcmd.sem_flg = SEM_UNDO; /* short sem_flg : IPC_NOWAIT, SEM_UNDO */
    if (semop(semid, &semcmd, 1) < 0)  {
        /* semop : semaphore operations 
        - int semid : semaphore ID
        - struct sembuf semop[] : contents of operation. array -> can do multiple operations. -1 -> decrease semaphore value
        - size_t nops : number of operations = 1
        */
		perror("semop");
        return -1;
    }
 
    return 0;
}
 
int
semTryWait(int semid) /* decrease semaphore value or return fail */
{
    struct sembuf   semcmd;
 
    semcmd.sem_num = 0;
    semcmd.sem_op = -1;
    semcmd.sem_flg = IPC_NOWAIT | SEM_UNDO; /* different from sem_flag : add IPC_NOWAIT*/
    /*
    if IPC_NOWAIT is not specified, the calling process is suspened until the semaphore's value >= the absolute value of sem_op
    if IPC_NOWAIT is specified, return is made with an error of EAGAIN
    */
    if (semop(semid, &semcmd, 1) < 0)  {
		perror("semop");
        return -1;
    }
 
    return 0;
}

int
semPost(int semid) /* increase semaphore value */
{
    struct sembuf   semcmd;
 
    semcmd.sem_num = 0;
    semcmd.sem_op = 1; /* different point from semWait - increase the semaphore value -> +1 */
    semcmd.sem_flg = SEM_UNDO;
    if (semop(semid, &semcmd, 1) < 0)  {
		perror("semop");
        return -1;
    }
 
    return 0;
}

int
semGetValue(int semid) /* get semaphore value */
{
    union semun {
            int     val;
    } dummy;
 
    return semctl(semid, 0, GETVAL, dummy); /* the cmd of semctl is GETVAL -> return the semaphore value */
}
 
int
semDestroy(int semid) /* remove the semaphore */
{
    union semun {
            int     val;
    } dummy;
 
    if (semctl(semid, 0, IPC_RMID, dummy) < 0)  { /* the cmd of semctl is IPC_RMID -> remove the semaphore set from the system*/
		perror("semctl");
        return -1;
    }
    close(semid);
 
    return 0;
}