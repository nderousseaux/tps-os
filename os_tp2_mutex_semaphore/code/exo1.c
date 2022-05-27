#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

long int compteur = 0;
pthread_mutex_t mutex;

void raler(const char* msg){
    perror(msg);
    exit(1);
}

void *fonction(void *arg){
    for (int i = 0; i<1000000; i++){
        if (errno = pthread_mutex_lock (&mutex) > 0)
        {raler("Erreur mutex_lock");}
        compteur++;
        if (errno = pthread_mutex_unlock (&mutex) > 0)
        {raler("Erreur mutex_lock");}
    }

    pthread_exit(NULL);
}

int main(int argc, char* argv[]){

   if (errno = pthread_mutex_init (&mutex, NULL) > 0)
   {raler("Erreur mutex_init");}


    //On prépare la place pour 4 pthread_t
    pthread_t *tid= malloc(4*sizeof(pthread_t));
    if (tid == NULL) raler("Erreur malloc");   

    for(int i = 0; i<4; i++){
        if (errno = pthread_create(&tid[i], NULL, fonction, NULL) > 0)
        {raler("Erreur pthread_create");}
    }

    //On attend la fin des threads
    for(int i=0; i<4; i++){
      pthread_join(tid[i], NULL);
    }

    printf("État compteur %ld \n", compteur);

    free(tid);
    pthread_mutex_destroy(&mutex);

    exit(0);
}