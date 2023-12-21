#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int count = 0;

void SigIntHandler(int signo)
{
	printf("%d\n",count);
    	exit(0); 
}

int main() {
	pid_t pid;
	
	signal(SIGINT, SigIntHandler);
	if ((pid = fork())< 0) {
    		perror("Fork failed");
    		return 1;
	} else if (pid == 0) {
		sleep(1);
		kill(getppid(), SIGINT);
		exit(0);
	} else {
		while(1) {
			count += 1;
		}
	}
	return 0;
}
