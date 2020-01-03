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
#include "semaphore.c"
#define FILENAME "story"
#define SEG_SIZE 489


int main(int argc, char * argsv[]) {
  //printf("%d\n", argc);
  int val, status, shmd, semd;
  int fd;
  char text[SEG_SIZE];
  //if command line input entered:
  if (argc > 1) {
    if (strcmp(argsv[1], "-c")==0) {
      //printf("Creating semaphore...\n");

      //CREATE SEMAPHORE
      val = create_sem();
      if (val < 0) printf("Error creating semaphore! %s", strerror(errno));
      else printf("Created semaphore successfully.\n");

      //CREATE SHARED MEMORY
      status = create_mem();
      if (status < 0) printf("Error creating shared memory! %s", strerror(errno));
      else {
        shmd = status; //only set shmd to the address if memory successfully created.
        printf("created shared memory successfully!\n");
      }

      //CREATE STORY FILE
      //printf("memory pointer: %d\n", shmd);
      fd = open(FILENAME, O_CREAT | O_TRUNC, 0644);
      if (fd < 0) printf("Error creating file. %s", strerror(errno));
      else printf("created file successfully!\n\n");
    }

    //READ + REMOVE
    if (strcmp(argsv[1], "-r")==0) {
      semd = get_semval();
      //printf("semd: %d\n", semd);

      printf("trying to get in\n");

      dec_sem(semd);

      //read from file
      fd = open(FILENAME, O_RDONLY );
      printf("The story so far: \n");
      while (read(fd, text, SEG_SIZE)) {
        printf("%s\n", text);
      }

      //remove SEMAPHORE
      semd = get_semval();
      shmd = get_shmd();

      del_mem(shmd);
      if (shmd == -1) printf("error removing shared memory: %s", strerror(errno));
      else printf("removed shared memory\n");

      status = close(fd);
      if (status == -1) printf("error closing file: %s", strerror(errno));
      else printf("removed file\n");

      del_sem(semd);
      if (semd == -1) printf("error removing semaphore: %s", strerror(errno));
      else printf("removed semaphore\n\n");


    }

    //VIEW
    if (strcmp(argsv[1], "-v")==0) {
      //read from file
      fd = open(FILENAME, O_RDONLY );
      printf("The story so far: \n");
      while (read(fd, text, SEG_SIZE)) {
        printf("%s\n", text);
      }
      printf("\n");
      return 0;

    }

  }
  return 0;
}
