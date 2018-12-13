#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <errno.h>

#define KEY 0xDEADBEEF

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
   if (argc > 2) {
     perror("Too many arguments, please try again");
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
   /*char * option = argv[1];

   if (strcmp(option, "-r") == 0) {
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
    perror("Shmctl");
    exit(1);
  }
   }
   if (strcmp(option, "-v") == 0) {
     if ()
   }
   if (strcmp(option, "-c") == 0) {
     // Create file //
   }*/
   return 0;
 }