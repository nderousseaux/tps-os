#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

struct args {int p; int t;};

void *fonction(void *arg){
  struct args *a = arg; //On stocke l'adresse de arg
  int *res = malloc(sizeof(int)); //On crée un pointeur vers un int
  *res = 0;

  for (int i=1; i<= a->p; i++){
		*res += ((a->t-1) * a->p+i);    
  }
  return res; //On renvoie le pointeur res
}


int main(int argc, char* argv[]){
  //On prend N et P
  int N = atoi(argv[1]);
  int P = atoi(argv[2]);
  int res = 0;

  //On prépare un tableau de N structures
  struct args *T_a = malloc(N*sizeof(struct args));
  
  //On prépare la place pour N pthread_t
  pthread_t *tid = malloc(N*sizeof(pthread_t));

  for(int i = 0; i<N; i++){
    //On met les bonnes valeurs dans les structure
    T_a[i].p = P;
    T_a[i].t = i+1;

    pthread_create(&tid[i], NULL, fonction, &T_a[i]);
  }
  
  //On attend la fin des threads
  for(int i=0; i<N; i++){
    void *resThread; //Type renvoyé par le thread
    pthread_join(tid[i], &resThread);
    int *resThreadInt = resThread; //On transforme le type *void en type *int
    res+= *resThreadInt; 
    free(resThread); //On libère la mémoire
  }

  free(T_a); //On libère la mémoire
  free(tid);

  printf("Le résultat est %d\n", res);
 
}