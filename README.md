# 2023-2 Linux System Programming
---

### 🔎 INDEX
#### 1. Introduction to Linux
#### 2. Getting Started with Linux
#### 3. Development Tools in Linux 
#### 4. Quick Introduction to C (hw2)

#### 5. File I/O (hw3)
- open, close, read, write
- lseek, create, fcntl, ioctl
- fopen, fclose, fread, fwrite
- fgetc, fputc, fgets, fputs
- fscanf, fprintf, sscanf, sprintf
- fseek, ftell, rewind
- ungetc, ferror, feof, fflush
  
#### 6. Files and Directories (hw4)
- stat, fstat, lstat
- umask
- chmod, fchmod
- chown, fchown, lchown
- link, symlink
- remove, rename
- mkdir, rmdir
- opendir, closedir, readdir, rewinddir
- chdir, fchdir
- sync, fsync
- getcwd

#### 7. Processes and Threads (hw5)
- getpid, getppid, getuid, geteuid, getgid, getegid
- fork
- exit, atexit
- wait, waitpid
- execel, execv, execle, execve, execlp, execvp
- system
- pthread_create, pthread_exit, pthread_join

#### 8. Signals (hw6)
- signal
- kill, raise
- sleep, alarm, pause
- pthread_cancel
- pthread_setcancelstate, pthread_setcalceltype

#### 9. Inter-Process Communication (hw7)
- **Pipe**: pipe, read, write, close
- **FIFO**: mkfifo, open, read, write, close
- **Message queue**: msgget, msgctl, msgsnd, msgrcv
- **Shared memory**: shmget, shmctl, shmat, shmdt
- **Memory-mapped file**: mmap, munmap

#### 10. Synchronization (hw8)
- **System V semaphore** : semget, semctl, semop
- **POSIX semaphore** : sem_init, sem_wait, sem_trywait, sem_post, setm_getvalue, sem_destroy
- **Mutex** : pthread_mutex_init, pthread_mutex_destroy, pthread_mutex_lock, pthread_mutex_trylock, pthread_mutex_unlock
- **Condition variables** : pthread_cond_init, pthread_cond_destroy, pthread_cond_wait, pthread_cond_signal, pthread_cond_breadcast

#### 11. Sockets (hw9)
- **TCP server** : socket, bind, listen, accept, read, write, close
- **TCP client** : socket, connect, read, write, close
- **UDP server** : socket, bind, recvfrom, sendto, close
- **UDP client** : socket, sendto, recvfrom, close
  
#### 12. Miscellaneous 

---
### 📄 Flipped Learning 정리 자료
- [ Quick Introduction to C ](https://pewter-pamphlet-a4a.notion.site/lsp-Quick-Introduction-to-C-e7c11a32882348e9a59d539db3cf747b?pvs=4)
- [ Files and Directories ](https://pewter-pamphlet-a4a.notion.site/LSP-Files-and-Directories-cf76fa6ec36c464eaafad699269b304d?pvs=4)
- [ IPC (2) ](https://pewter-pamphlet-a4a.notion.site/LSP-IPC-2-beb7f415d6564149a204921cc6140acf?pvs=4)
