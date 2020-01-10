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
#include <sys/shm.h>
#include "semaphore.c"

int main() {
  char * text;
  char input[SEG_SIZE];
  int shmd = shmget(SHM_KEY, SEG_SIZE, 0600);
  int semd = get_semval();
  int status;

  if (shmd  == -1) printf("Error shmding: %s", strerror(errno));

  printf("trying to get in\n");
  dec_sem(semd);
  printf("success!\n");

  //shmat to get last addition
  text = shmat(shmd, NULL, 0);
  //text = "TEST";
  if (text < 0) printf("Error: %s", strerror(errno));
  // printf("hi\n");
  printf("Last addition: %s\n", text);
  printf("Your addition: ");
  fgets(input, SEG_SIZE, stdin);
  if (status < 0) printf("Error: %s", strerror(errno));
  //printf("hi\n");
  *strchr(input, '\n') = 0;

  //write input to FILE
  int fd = open(FILENAME, O_WRONLY | O_APPEND);
  write(fd, input, SEG_SIZE);
  close(fd);

  //save text into shmd
  strcpy(text, input);
  status = shmdt(text);
  if (status == -1) printf("error shmdting: %s", strerror(errno));
  inc_sem(semd);
  printf("\n");


  return 0;



}
