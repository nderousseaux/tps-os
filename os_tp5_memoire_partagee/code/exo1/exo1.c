#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#define CHK(op)         do { if ((op) == -1) { raler (#op); } } while (0)
void raler (const char *msg){
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]){

    if (argc != 2){
        fprintf(stderr, "Usage : ./exo1 file_name \n");
        return EXIT_FAILURE;
    }
    char * file_name = argv[1];


    //On ouvre un fichier
    int file;
    CHK(file = open(file_name, O_RDONLY));
    
    //On récupère la taille du fichier
    struct stat stat_file; 
    CHK(fstat(file, &stat_file));

    //On le projette en mémoire
    char * file_mem;
    file_mem = mmap(NULL, stat_file.st_size, PROT_READ, MAP_PRIVATE, file, 0);
    if(file_mem == MAP_FAILED){
        raler("Mmap failed");
    }

    //On ferme le fichier
    CHK(close(file));

    //TODO: Algo

    CHK(munmap(file_mem, stat_file.st_size));
    exit(EXIT_SUCCESS);
}