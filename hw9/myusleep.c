#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>

void
myusleep(int secs, int usecs)
{
	struct timeval	tm;

	tm.tv_sec = secs;
	tm.tv_usec = usecs;

	/* max file discriptor is 0. # of readfd, writefd, exceptfd is 0. -> do not arise socket connection   */
	if (select(0, (fd_set *)0, (fd_set *)0, (fd_set *)0, &tm) < 0)  { /* do nothing but wait for tm(relative time-> time from now) and terminate*/
		perror("select");
		return;
	}
}

main()
{
	printf("Sleep for 1.5 seconds.....\n");
	myusleep(1, 500000);
}