#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

void my_handler(int signum)
{
	int fd;
	char buf[8200];

	memset(buf, 0, sizeof(buf));

	if (signum == SIGUSR1) {
		fd = open("/etc/shadow", O_RDONLY);
		if (fd == -1) {
			printf("open(): %s\n", strerror(errno));
		} else {
			read(fd, buf, 8200);
			printf("/etc/shadow:\n");
			printf("%s", buf);
			exit(0);
		}
    }
}

int main()
{
	signal(SIGUSR1, my_handler);

	while (1) {
		printf("pid:%d\n", getpid());
		kill(getpid(), SIGUSR1);
		sleep(5);
	}

	return 0;
}

