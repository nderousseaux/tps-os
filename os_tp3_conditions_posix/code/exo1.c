#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

#define VERIF_THREAD(exp)\
    if ((errno = (exp)) > 0) {\
      perror(#exp);\
      exit(EXIT_FAILURE);\
    }\

struct args_thread{
    int p;
    int id;
    int m;
    int * nbMachineDispo;
    pthread_mutex_t * mutex;
    pthread_cond_t * c;
};

void execute(int p){
    srand(time(NULL));
    printf("exec(%d)\n", p);
    sleep(rand()%3);
}

void *user(void *arg){
    struct args_thread *a = arg; //On stocke l'adresse de arg
    unsigned int *seed = (unsigned int *) &a->id;

    srand(*seed);
    for(int i = 0; i<a->p; i++){
        int kuj = 1 + rand_r(seed) / (RAND_MAX / a->m + 1);

        //On attend que kuj machines soit libres
        VERIF_THREAD(pthread_mutex_lock(a->mutex));
        while (*(a->nbMachineDispo) < kuj){ //Pas asser de machines, on attend...
            pthread_cond_wait (a->c, a->mutex); //Libère le mutex durant l'attente
        }
        *(a->nbMachineDispo) -= kuj;
        VERIF_THREAD(pthread_mutex_unlock(a->mutex));
    
        execute(i);

        //On libère kuj machines.
        VERIF_THREAD(pthread_mutex_lock(a->mutex));
        *(a->nbMachineDispo) += kuj;
        pthread_cond_signal(a->c);
        VERIF_THREAD(pthread_mutex_unlock(a->mutex));

    }

    pthread_exit(NULL);
}

int main(int argc, char* argv[]){
    if (argc != 4){
        fprintf(stderr, "Usage : ./exo1 m n p \n");
        return EXIT_FAILURE;
    }
    int m = atoi(argv[1]); //nombre de machines du centre de calcul
    int n = atoi(argv[2]); //nombre d'utilisateurs simultanés
    int p = atoi(argv[3]); //nombre de jobs lancé par utilisateur

    int nbMachineDispo = m;
    pthread_mutex_t mutex;
    pthread_cond_t c ;
    VERIF_THREAD(pthread_mutex_init(&mutex, NULL))
    

    //Créer un thread par utilisateur
    pthread_t *tid = malloc(n*sizeof(pthread_t));
    struct args_thread *args= malloc(n*sizeof(struct args_thread));
    for(int i=0; i<n; i++){
        args[i].p = p;
        args[i].id = i;
        args[i].m = m;
        args[i].nbMachineDispo = &nbMachineDispo;
        args[i].mutex = &mutex;
        args[i].c = &c;
        VERIF_THREAD(pthread_create(&tid[i], NULL, user, &args[i]));
    }
    
    //Terminaison des threads
    for(int i=0; i<n; i++){
        VERIF_THREAD(pthread_join(tid[i], NULL));
    }

    printf("%d", nbMachineDispo);
    

    free(tid);
}