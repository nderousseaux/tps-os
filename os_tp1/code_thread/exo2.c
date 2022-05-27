// Dans cet exercice, le calcul est simplifié à l’extrême : on considère une chaîne de n caractères. 
// Chaque thread i (avec 0 ≤ i < n) remplit la case d’indice i de cette chaîne avec alternativement le caractère « # » ou « - ». 
// Lorsque les n threads ont écrit leur caractère, l’un deux (n’importe lequel) se charge d’afficher la chaîne ainsi calculée, 
// puis on commence une nouvelle phase de calcul, pour un total de p itérations.
// L’algorithme de chaque thread i est donc constitué d’une boucle parcourue p fois : à chaque tour, 
// le thread remplit la case i de la chaîne avec « # » ou « - », puis attend que les n − 1 autres threads aient rempli leur propre case, 
// et enfin l’un des threads affiche la chaîne une fois l’attente terminée.
// La sortie du programme (avec n = 20 et p = 2) sera :
//                         ####################
//                         --------------------
//                         ####################
//                         --------------------
// (une ligne de #) (une ligne de -) (une ligne de #) (une ligne de -)
// Votre programme prendra n et p en arguments. Vous n’utiliserez bien sûr aucune variable globale.

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

struct args {
    int *p;
    int i; //Numero du thread 
    char *sortie; //Chaine de sortie
    pthread_barrier_t *bar;
};

void raler(const char* msg){
    perror(msg);
    exit(1);
}

void *fonction(void *arg){
  struct args *a = arg; //On stocke l'adresse de arg

  char cara[2]; cara[0] = '#'; cara[1] = '-';

  for(int i=0; i<*(a->p); i++){

    //On rempli la chaine
    a->sortie[a->i] = cara[i%2];

    int rep = pthread_barrier_wait(a->bar);
    if(rep == PTHREAD_BARRIER_SERIAL_THREAD){
          printf("%s\n", a->sortie);
    }

    //On attend que le thread ai affiché le message, sinon les autres vont le modifier.
    pthread_barrier_wait(a->bar);
  }
}

int main(int argc, char* argv[]){
    //On prend n et p
    if (argc != 3){
        raler("Usage : ./exo2 n p");
    }
    int n = atoi(argv[1]);
    int p = atoi(argv[2]);
    pthread_barrier_t bar;
    if((errno = pthread_barrier_init(&bar, NULL, n))>0){
        raler("Erreur init barrière");
    }

    //On prépare la place pour N pthread_t
    pthread_t *tid= malloc(n*sizeof(pthread_t));
    if (tid == NULL) raler("Erreur malloc");   

    //On prépare un tableau de N structures
    struct args *T_a = malloc(n*sizeof(struct args));
    if (T_a == NULL) raler("Erreur malloc");    

    //On prépare la chaine de sortie
    char *sortie= malloc((n+1)*sizeof(char));
    if (sortie == NULL) raler("Erreur malloc"); 
    sortie[n] = '\0';

    for(int i = 0; i<n; i++){
      //On rempli la structure
      T_a[i].p = &p;
      T_a[i].i = i;
      T_a[i].sortie = sortie;   
      T_a[i].bar = &bar;
      pthread_create(&tid[i], NULL, fonction, &T_a[i]);
    }
  
    //On attend la fin des threads
    for(int i=0; i<n; i++){
      pthread_join(tid[i], NULL);
    }

    free(tid);  
    free(T_a);
    free(sortie);
    pthread_barrier_destroy(&bar);
    exit(0);
}