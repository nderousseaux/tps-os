#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

void raler(const char* msg){
    perror(msg);
    exit(1);
}

void tache(int i){
  printf("Tâche n°%d \n", i);
}

void *fonction31(){
  //Tâche n°31
  tache(31);
}

void *fonction32(){
  //Tâche n°32
  tache(32);
}

void *fonction33(){
  //Tâche n°33
  tache(33);
}

void *fonction42(){
  //Tâche n°42
  tache(42);
}

void *fonction34(){
  //Tâche n°34
  tache(34);

  pthread_t tid = malloc(sizeof(pthread_t));
  if (tid == NULL) raler("Erreur malloc");

  pthread_create(&tid, NULL, fonction42, NULL);

  pthread_join(tid, NULL);
}

void *fonction22(){
  //Tâche n°22
  tache(22);

  pthread_t tid = malloc(sizeof(pthread_t));
  if (tid == NULL) raler("Erreur malloc");

  pthread_create(&tid, NULL, fonction34, NULL);

  pthread_join(tid, NULL);
}

void *fonction21(){
  //Tâche n°21
  tache(21);

  pthread_t *tid = malloc(3*sizeof(pthread_t));
  if (tid == NULL) raler("Erreur malloc");

  pthread_create(&tid[0], NULL, fonction31, NULL);
  pthread_create(&tid[1], NULL, fonction32, NULL);
  pthread_create(&tid[2], NULL, fonction33, NULL);

  pthread_join(tid[0], NULL);
  pthread_join(tid[1], NULL);
  pthread_join(tid[2], NULL);

  free(tid);

  //Tâche 41
  tache(41);
}

int main(int argc, char* argv[]){

  //Tâche n°1
  tache(1);

  //On divise la tâche en deux
  pthread_t *tid = malloc(2*sizeof(pthread_t));
  if (tid == NULL) raler("Erreur malloc");

  pthread_create(&tid[0], NULL, fonction21, NULL);
  pthread_create(&tid[1], NULL, fonction22, NULL);

  pthread_join(tid[0], NULL);
  pthread_join(tid[1], NULL);
  

  free(tid);

  //Tâche n°5
  tache(5);

  printf("Fin du programme");

  exit(0);
}