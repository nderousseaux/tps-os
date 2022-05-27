#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <semaphore.h>

//Une case du tableau Donnée 
struct Donnee{
    int data; //Contenu de la case
    int n; //Numéro de la donnée produite
};  

//Variables globales
int D; //Nb de données crées par producteur
int B; //Taille du tampon
struct Donnee *tampon;
sem_t sem_ecrites; //Compte le nombre de cases écrites
sem_t sem_libres; //Compte le nombre de cases libres
int compteur_n = 0; //Compte le nombre de données produites
int indice_produit = 0; //Tête d'écriture
int indice_conso = 0; //Tête de lecture

pthread_mutex_t verou_ecriture; //Verrou pour indice_produit et compteur_n
pthread_mutex_t verou_lecture; //Verrou pour indice_conso

void raler(const char* msg){
    perror(msg);
    exit(1);
}

void creer_une_donnee(int data){
    //On crée l'objet "donnée"
    struct Donnee d;
    d.data = data;
    
    sem_wait(&sem_libres); //On attend qu'une case soit libre
    
    if(errno = pthread_mutex_lock(&verou_ecriture) > 0){
        raler("Erreur mutex lock");
    }
    d.n = compteur_n;
    //On l'ajoute au buffer
    tampon[indice_produit] = d;
    //On déplace la tête d'écriture de 1
    indice_produit = (indice_produit + 1) % B;
    compteur_n++;
    if(errno = pthread_mutex_unlock(&verou_ecriture) > 0){
        raler("Erreur mutex unlock");
    }
    sem_post(&sem_ecrites); //On indique qu'un case à été écrite
    if(d.data != -1 ){
        printf("P%d (%d)\n", d.n, d.data);
    }

}

void *producteur(void *arg){
    //On crée D données
    for(int i=0; i<D; i++){
        int data = rand();
        creer_une_donnee(data);   
    }
    pthread_exit(NULL);
}


void *consommateur(void *arg){
    while(1){
        struct Donnee d;
        sem_wait(&sem_ecrites); //On attend qu'une case soit écrite

        if(errno = pthread_mutex_lock(&verou_lecture)> 0){
            raler("Erreur mutex lock");
        }
        d = tampon[indice_conso];
        indice_conso = (indice_conso + 1) % B;
        if (d.data != -1){
            printf("C%d (%d)\n", d.n, d.data);
        }
        if(errno = pthread_mutex_unlock(&verou_lecture)>0){
            raler("Ereur mutex unlock");
        }
        sem_post(&sem_libres); //On indique qu'une case est libre

        if(d.data == -1)
            break;
    }
    pthread_exit(NULL);
}

int main(int argc, char* argv[]){
    if (argc != 5){
        raler("Usage : ./exo2 D P C B");
    }
    D = atoi(argv[1]); //Nb de données crées par producteur
    int P = atoi(argv[2]); //Nb de producteurs
    int C = atoi(argv[3]); //Nb de consommateurs 
    B = atoi(argv[4]); // Taille du tampon
    srand( time( NULL ) );

    //On crée le tampon
    tampon = malloc(B*sizeof(struct Donnee));
    if (tampon == NULL) raler("Erreur malloc");

    //On crée le sémaphore ecrites qui compte le nombre de cases écrites
    if (errno = sem_init(&sem_ecrites, 0, 0) > 0){
        raler("Erreur sem_init");
    }

    //On crée le sémaphore libres qui compte le nombre de cases libres
    if (errno = sem_init(&sem_libres, 0, B) > 0){
        raler("Erreur sem_init");
    }

    //On crée le verrou de la tête d'écriture
    if (errno = pthread_mutex_init(&verou_ecriture, NULL) > 0){
        raler("Erreur mutex_init");
    }

    //On crée le verrou de la tête de lecture
    if (errno = pthread_mutex_init(&verou_lecture, NULL) > 0){
        raler("Erreur mutex_init");
    }

    //On crée les P threads producteurs
    pthread_t *tProd= malloc(P*sizeof(pthread_t));
    if (tProd == NULL) raler("Erreur malloc");   
    for(int i = 0; i<P; i++){
        pthread_create(&tProd[i], NULL, producteur, NULL);
    }

    //On crée les C threads consommateur
    pthread_t *tCons= malloc(C*sizeof(pthread_t));
    if (tCons == NULL) raler("Erreur malloc");   
    for(int i = 0; i<C; i++){
        pthread_create(&tCons[i], NULL, consommateur, NULL);
    }

    //On attend la fin des threads
    for(int i=0; i<P; i++){
      pthread_join(tProd[i], NULL);
    }
    
    for (int i = 0; i < C; i++)
    {
        creer_une_donnee(-1);
    }

    for(int i=0; i<C; i++){
        pthread_join(tCons[i], NULL);
    }

    //On fait du ménage
    free(tProd);
    pthread_mutex_destroy(&verou_ecriture);
    pthread_mutex_destroy(&verou_lecture);
    sem_destroy(&sem_ecrites);
    sem_destroy(&sem_libres);


    exit(0);
}