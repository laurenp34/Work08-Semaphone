#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>
#include <semaphore.h>
#include <sys/sem.h>

int main(){
	sem_t sem;
	int pshared;
	int ret;
	int value;

	/* initialize a private semaphore */
	pshared = 0;
	value = 1;
	ret = sem_init(&sem, pshared, value);
	
	if (ret == -1) {
		printf("error creating semaphore: %s", strerror(errno));
		return 0;
	}
	printf("created semaphore successfully!\n");
	sem_getvalue(&sem, &value);
	
	printf("value: %d\n", value); 
	return 0;

}
