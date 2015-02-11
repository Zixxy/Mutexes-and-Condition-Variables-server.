#include <stdio.h>
#include <unistd.h>
#include <cv_sys.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>

void sighandler(int sig) {}

int main(int argc, char **argv) {
	int mut;
	pid_t pids[10], child;
	int i;
	mut = atoi(argv[1]);
	signal(SIGUSR1, &sighandler);
	cs_lock(mut);
	for (i = 0; i < 10; ++i) {
		sleep(1);
		if (child = fork()) {
			pids[i] = child;
		}
		else {
			errno = 0;
			cs_lock(mut);
			if (errno == EINTR) {
				printf("%d int\n", i);
			}
			else {
				printf("%d got\n", i);
			}
			cs_unlock(mut);
			if (errno == EPERM) {
				printf("%d per\n", i);
			}
			exit(0);
		}
	}
	sleep(5);
	for (i = 0; i < 10; i += 2) {
		kill(pids[i], SIGUSR1);
	}
	sleep(1);
	cs_unlock(mut);
	sleep(1);
	return 0;
}
