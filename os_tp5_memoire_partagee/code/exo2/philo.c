#include <stdio.h>
#include <stdlib.h>

#define CHK(op)         do { if ((op) == -1) { raler (#op); } } while (0)
void raler (const char *msg){
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]){

    if (argc != 2){
        fprintf(stderr, "Usage : ./philo nrepas \n");
        return EXIT_FAILURE;
    }
    int nrepas = atoi(argv[1]);

   exit(EXIT_SUCCESS);
}