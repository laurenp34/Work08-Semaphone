#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#define SEM_KEY 72
#define SHM_KEY 149
#define SEG_SIZE 489
#define FILENAME "story"

// union semun {
// 					 int              val;    /* Value for SETVAL */
// 					 struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
// 					 unsigned short  *array;  /* Array for GETALL, SETALL */
// 					 struct seminfo  *__buf;  /* Buffer for IPC_INFO
// 																			 (Linux-specific) */
// };

//creates semaphore, returns the return value of semget
int create_sem() {
	int semd = semget(SEM_KEY, 1, IPC_CREAT | IPC_EXCL | 0644);
	int r;

	//if semaphore w key already exists:
	if (semd == -1) {
		//printf("error creating semaphore\n");
		semd = semget(SEM_KEY,1,0);
		r = semctl(semd,0,GETVAL,0);
		//printf("semctl returned: %d\n",r);
	} else {
		union semun us;
		us.val = 1;
		r = semctl(semd, 0, GETVAL, 0);
		//printf("semctl returned: %d\n",r);
	}

	return semd;
}

int create_mem() {
	int shmd;
	shmd = shmget(SHM_KEY, SEG_SIZE, IPC_CREAT | 0644);
	// if (shmd < 0) printf("Error creating shared memory! %s\n", strerror(errno));
	// else printf("Success creating shared memory!\n");
	return shmd;
}

int del_sem(int semd) {
	int out = semctl(semd, IPC_RMID, 0);
	return out;
}

int del_mem(int shmd) {
	int out = shmctl(shmd, IPC_RMID, 0);
	return out;
}

int get_semval() {
	int semd = semget(SEM_KEY, 1, 0);
	return semd;
}

int get_shmd() {
	int out = shmget(SHM_KEY, SEG_SIZE, 0644);
	return out;
}

//trying to get in
int dec_sem(int semd) {
	int out;
	//reduce sem by 1:
	struct sembuf op;
	op.sem_num = 0;
	op.sem_op = -1;
	out = semop(semd, &op, 1);

	return out;
}

//done w operation, exiting
int inc_sem(int semd) {
	int out;
	struct sembuf op;
	op.sem_num = 0;
	op.sem_op = 1;
	out = semop(semd, &op, 1);

	return out;
}

// int main(){
// 	int i;
// 	i = create_sem();
// 	printf("i: %d\n", i);
// 	i = create_sem();
// 	printf("i: %d\n", i);
//
// 	i = get_semval();
// 	printf("i: %d\n", i);
//
// }
