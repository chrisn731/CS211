#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>


void arr_free(int **arr, int dim) {
    int i;
    for(i = 0; i < dim; i++)
        free(arr[i]);
    free(arr);
}


void arr_print(int **arr, int dim) {
    int i, j; 
    for(i = 0; i < dim; i++) {
        for(j = 0; j < dim; j++) 
            printf((j == dim - 1) ? "%d" : "%d ", arr[i][j]);
        printf("\n");
    }
}


void exponentiate(int **arr, int exp, int dim) {
    int i, j, k, n;
    if(!exp) {
        for(i = 0; i < dim; i++)
            for(j = 0; j < dim; j++)
                arr[i][j] = (i == j) ? 1 : 0;
        return;
    }
    
    int base[dim][dim]; 
    for(i = 0; i < dim; i++)
        for(j = 0; j < dim; j++)
            base[i][j] = arr[i][j];
    
    for(n = 1; n < exp; n++) {
        int mul[dim][dim];
        /** memset(mul, 0, sizeof(int) * dim * dim); */
        
        for(i = 0; i < dim; i++) {
            for(j = 0; j < dim; j++) {
                mul[i][j] = 0; 
                for(k = 0; k < dim; k++)
                    mul[i][j] += arr[i][k] * base[k][j];
            } 
        }
        for(i = 0; i < dim; i++)
            for(j = 0; j < dim; j++)
                arr[i][j] = mul[i][j];
    }
}


int main(int argc, char **argv) {
    if(argc < 2) {
        perror("Need file\n");
        return -1;
    }
    
    char *c = NULL;
    size_t n = 0;
    FILE *fd = fopen(argv[1], "r");
    
    int dim, exp;
    if(getline(&c, &n, fd)) {
        dim = atoi(c); 
    } else {
        perror("Cannot get dimension\n");
        return -1;
    }

    int **arr = (int **) malloc(dim * sizeof(int *));
    int i;
    for(i = 0; i < dim; i++)
        arr[i] = (int *) malloc(dim * sizeof(int));

    for(i = 0; i < dim; i++) {
        int j = 0; 
        if(getline(&c, &n, fd) != -1) {
            char *ptr = strtok(c, " ");
            while(ptr) {
                arr[i][j] = atoi(ptr);
                ptr = strtok(NULL, " ");
                j++;
            }
        }
    }

    if(getline(&c, &n, fd)) {
        exp = atoi(c);
    } else {
        perror("Cannot get exponent\n");
        return 1;
    }
    
    exponentiate(arr, exp, dim);
    arr_print(arr, dim);
    arr_free(arr, dim);
    fclose(fd);
    free(c);
    return 0;
}