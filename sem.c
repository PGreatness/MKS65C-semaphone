#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <errno.h>

#define KEY 0xDEADBEEF

int main() {
  int semd;
  semd = semget(KEY, 1, 0);

  struct sembuf sb;
  sb.sem_num = 0;
  sb.sem_flg = SEM_UNDO;
  sb.sem_op = -1;
  semop(semd, &sb, 1);

  printf("Got the semaphore!\n");
  sleep(1);

  sb.sem_op = 1;
  semop(semd, &sb, 1);

  return 0;
}
