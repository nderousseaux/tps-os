#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define CHK(op)         do { if ((op) == -1) { raler (#op); } } while (0)
struct sharedMemory{
    int nbCouverts;
    int nextPhilo;
};


void raler (const char *msg){
    perror(msg);
    exit(EXIT_FAILURE);
}

/*
Ce programme prend en paramètre le nombre de couverts à dresser (c’est-à-dire le nombre de philosophes, de bols ou de baguettes)

Il initialise un segment de mémoire partagée nommé /table contenant, a minima, le nombre de couverts, 
le numéro du prochain philosophe attendu à la table (initialement nul, incrémenté à chaque fois qu’un philosophe s’attable),
et autant de sémaphores que nécessaire.

Une fois la table dressée, le programme peut se terminer. 
Le nombre de couverts ne doit être borné que par la mémoire disponible et non par une limite imposée par votre programme.
*/
int main(int argc, char* argv[]){

    if (argc != 2){
        fprintf(stderr, "Usage : ./table nphilo \n");
        return EXIT_FAILURE;
    }
    int nbPhilo = atoi(argv[1]);
    if(nbPhilo < 0){
        fprintf(stderr, "nphilo should be greater than 0\n");
        return EXIT_FAILURE;
    }



    int fd;
    CHK(fd = shm_open("/table", O_CREAT, S_IRWXU));
    CHK(ftruncate(fd, sizeof(struct sharedMemory)));
    struct sharedMemory *args = mmap(NULL, sizeof(struct sharedMemory), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(args == MAP_FAILED){
        raler("Erreur mmap");
    }

    //On crée les données
    args->nbCouverts = nbPhilo;
    args->nextPhilo = 0;

    exit(EXIT_SUCCESS);
}