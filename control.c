#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <errno.h>
#include <fcntl.h>

#define KEY 0xBEADBEEF

union semun {
   int              val;    /* Value for SETVAL */
   struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
   unsigned short  *array;  /* Array for GETALL, SETALL */
   struct seminfo  *__buf;  /* Buffer for IPC_INFO */
};

int main(int argc, char const *argv[]) {
  int semd;
  int r;
  int v;
  int shmid;
  char * data;
  key_t key;
  FILE * f;

  if (argc != 2) {
    printf("Arguments do not match, please try again\n");
    exit(1);
  }

  if (strcmp(argv[1], "-c") == 0) {
    if ((shmid = shmget(KEY, sizeof(KEY), 0644 | IPC_CREAT)) == -1) {
      printf("Error %d: %s\n", errno, strerror(errno));
      exit(1);
    }

    if ((f = open("textfile", O_CREAT | O_TRUNC, 0644)) == -1) {
      printf("Error %d: %s\n", errno, strerror(errno));
    }
    if ((semd = semget(KEY, 1, IPC_CREAT | IPC_EXCL | 0644)) == -1) {
      printf("Error %d: %s\n", errno, strerror(errno));
      exit(1);
    }
    union semun us;
    us.val = 3;
    r = semctl(semd, 0, SETVAL, us);
    printf("Semctl returned: %d\n", r);

  }

  if (strcmp(argv[1], "-v") == 0) {
    if ((f = open("textfile", O_RDONLY, 0644)) == -1) {
      printf("Error %d: %s\n", errno, strerror(errno));
    }
    char reading[1024];
    int re = read(f, reading, 1023);
    if (re == -1) {
      printf("Error %d: %s\n", errno, strerror(errno));
      exit(1);
    }
    if (re != NULL) {
      printf("This was read:\n%s\n", re);
    }else{
      printf("Nothing read. Add something to begin.\n");
    }
  }

  if (strcmp(argv[1], "-r") == 0) {
    if ((semd = semget(KEY, 1, IPC_CREAT)) == -1) {
      printf("Error %d: %s\n", errno, strerror(errno));
      exit(1);
    }
    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_op = -1;
    sb.sem_flg = SEM_UNDO;

    int await;
    await = semop(semd, &sb, 1);

    if ((semctl(semd, 1, IPC_RMID)) == -1) {
      printf("Error %d: %s\n", errno, strerror(errno));
      exit(1);
    }
    if ((shmid = shmget(KEY, sizeof(KEY), 0644)) == -1) {
      printf("Error %d: %s\n", errno, strerror(errno));
      exit(1);
    }
    int rem;
    if ((rem = shmctl(shmid, IPC_RMID, NULL)) == -1) {
      printf("Error %d: %s\n", errno, strerror(errno));
      exit(1);
    }
    if (remove("textfile") == -1) {
      printf("Error %d:%s\n", errno, strerror(errno));
      exit(1);
    }
    printf("Semaphone removed. File deleted.\n");
  }

  /*if ((key = ftok("./textfile", 'R')) == -1) {
    perror("ftok");
    exit(1);
  }

  if ((shmid = shmget(key, 1024, 0644 | IPC_CREAT)) == -1) {

  }

  semd = semget(KEY, 1, IPC_CREAT | IPC_EXCL | 0644 );
  if (semd == -1) {
    printf("Error %d: %s\n", errno, strerror(errno));
    semd = semget(KEY, 1, 0);
    v = semctl(semd, 0, GETVAL, 0);
    printf("semctl returned: %d\n", v);
  }else{
    union semun us;
    us.val = 1;
    r = semctl(semd, 0, SETVAL, us);
    printf("semctl returned: %d\n", r);
  }
   /*char * argv[1] = argv[1];

  if (strcmp(argv[1], "-r") == 0) {
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
    perror("Shmctl");
    exit(1);
  }
  }
  if (strcmp(argv[1], "-v") == 0) {
    if ()
  }
  if (strcmp(argv[1], "-c") == 0) {
    // Create file //
   }*/
  return 0;
}