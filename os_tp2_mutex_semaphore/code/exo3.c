#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <semaphore.h>
#include <stdbool.h>

//Si le retour du thread est supérieur à 1 --> échec
#define TCHK(exp)\
  do {\
    if ((errno = (exp)) > 0) {\
      fprintf(stderr, "%s:%d: ", __func__, __LINE__);\
      perror(#exp);\
      exit(EXIT_FAILURE);\
    }\
  } while (0)

//Si le retour du sémaphore est égal à -1 --> échec
#define SCHK(exp)\
  do {\
    if ((exp) == -1) {\
      perror(#exp);\
      exit(EXIT_FAILURE);}\
  } while (0)

typedef struct //On crée une structure pour chaque sémaphore
{
  sem_t sema_11;
  sem_t sema_12;
  sem_t sema_13;
  sem_t sema_21;
  sem_t sema_22;
  sem_t sema_23;
  sem_t sema_24;
} semaphores;

semaphores semas;

void tache(int n){
    printf("Tâche n°%d\n", n);
}

void *do_tache(void * arg)
{
  int *num = arg;
  switch (*num){
    case 11:
        tache(*num);
        SCHK(sem_post(&semas.sema_11)); //On indique que la tâche 11 est finie (on libère une place pour 21 et 22)
        SCHK(sem_post(&semas.sema_11));
        break;

    case 12:
        tache(*num);
        SCHK(sem_post(&semas.sema_12)); //On indique que la tâche 12 est finie (on libère une place pour 21, 22 et 23)
        SCHK(sem_post(&semas.sema_12));
        SCHK(sem_post(&semas.sema_12));
        break;

    case 13:
        tache(*num);
        SCHK(sem_post(&semas.sema_13)); //On indique que la tâche 13 est finie (on libère une place pour 31, 23 et 24 )
        SCHK(sem_post(&semas.sema_13));
        SCHK(sem_post(&semas.sema_13));
        break;

    case 21:
        SCHK(sem_wait(&semas.sema_11)); //On attend que la tâche 11 soit finie
        SCHK(sem_wait(&semas.sema_12)); //On attend que la tâche 12 soit finie
        tache(*num);
        SCHK(sem_post(&semas.sema_21)); //On indique que la tâche 21 est finie (on libère une place pour 31)
        break;

    case 22:
        SCHK(sem_wait(&semas.sema_11)); //On attend que la tâche 11 soit finie
        SCHK(sem_wait(&semas.sema_12)); //On attend que la tâche 12 soit finie
        tache(*num);
        SCHK(sem_post(&semas.sema_22)); //On indique que la tâche 22 est finie (on libère une place pour 32)
        break;

    case 23:
        SCHK(sem_wait(&semas.sema_12)); //On attend que la tâche 12 soit finie
        SCHK(sem_wait(&semas.sema_13)); //On attend que la tâche 13 soit finie
        tache(*num);
        SCHK(sem_post(&semas.sema_23)); //On indique que la tâche 23 est finie (on libère une place pour 32)
        break;

    case 24:
        SCHK(sem_wait(&semas.sema_13)); //On attend que la tâche 13 soit finie
        tache(*num);
        SCHK(sem_post(&semas.sema_24)); //On indique que la tâche 24 est finie (on libère une place pour 32)
        break;

    case 31:
        SCHK(sem_wait(&semas.sema_13)); //On attend que la tâche 13 soit finie
        SCHK(sem_wait(&semas.sema_21)); //On attend que la tâche 21 soit finie
        tache(*num);
        break;

    case 32:
        SCHK(sem_wait(&semas.sema_22)); //On attend que la tâche 22 soit finie
        SCHK(sem_wait(&semas.sema_23)); //On attend que la tâche 23 soit finie
        SCHK(sem_wait(&semas.sema_24)); //On attend que la tâche 24 soit finie
        tache(*num);
        break;
  }
  
  pthread_exit(NULL);
}

int main(int argc, char* argv[]){
    
    //On initialise les sémaphores à 0 -> Les tâches n'ont pas commencés
    SCHK(sem_init(&semas.sema_11, 0, 0));
    SCHK(sem_init(&semas.sema_12, 0, 0));
    SCHK(sem_init(&semas.sema_13, 0, 0));
    SCHK(sem_init(&semas.sema_21, 0, 0));
    SCHK(sem_init(&semas.sema_22, 0, 0));
    SCHK(sem_init(&semas.sema_23, 0, 0));
    SCHK(sem_init(&semas.sema_24, 0, 0));

    pthread_t tid[9]; //On crée un thread par tâche
    int tab[] = {11, 12, 13, 21, 22, 23, 24, 31, 32};
    for(int i = 0; i < 9; i++){
        TCHK(pthread_create(&tid[i], NULL, do_tache, &tab[i]));
    }

    for (int i = 0; i < 9; i++){ //On attend la fin des threads
        TCHK(pthread_join(tid[i], NULL));
    }

    SCHK(sem_destroy(&semas.sema_11));
    SCHK(sem_destroy(&semas.sema_12));
    SCHK(sem_destroy(&semas.sema_13));
    SCHK(sem_destroy(&semas.sema_21));
    SCHK(sem_destroy(&semas.sema_22));
    SCHK(sem_destroy(&semas.sema_23));
    SCHK(sem_destroy(&semas.sema_24));

    return EXIT_SUCCESS;
}