#include <stdio.h>
#include <stdlib.h>

void printmatrix(int **matrix, int size);

int **createMatrix(int size){
	int **newmatrix = malloc(size * sizeof(int*));
	int i;
	for(i = 0; i < size; i++){
		newmatrix[i] = malloc(size * sizeof(int));
	}
	return newmatrix;
}

void freeMatrix(int ***matrix, int size){
	int i;
	for(i = 0; i < size; i++){
		free((*matrix)[i]);
	}
}

void setmatrix(int ***matrixA, int ***matrixB, int size){
	int i,j;
	for(i = 0; i < size; i++){
		for(j = 0; j < size; j++){
			(*matrixA)[i][j] = (*matrixB)[i][j];
		}
	}
}

int matrixmulti(int **matrixA, int power, int size){
	
	int **product = createMatrix(size);
	int **matrixB = createMatrix(size);

	int row, col, colplace, result = 0;

	setmatrix(&matrixB, &matrixA, size);
	
	/**
	 * row = which row to select the element from
	 * col = which column in the matrix
	 * colplace = which vector of A to select for 
	 * matrix[row][col] will be used as the horizontal vector of A
	 * matrix[col][colplace] will be used as the vertical vector of A
	 */
	while( (power - 1) > 0){
		for(row = 0; row < size; row++){
			for(colplace = 0; colplace < size; colplace++){
				for(col = 0; col < size; col++){
					result += matrixA[row][col] * matrixB[col][colplace];
				}
				product[row][colplace] = result;
				result = 0;
			}
		}
		power--;
		setmatrix(&matrixA, &product, size);
	}
	freeMatrix(&matrixA, size);
	freeMatrix(&matrixB, size);

	printmatrix(product, size);
	freeMatrix(&product, size);
	return 1;
}

void IMatrix(int size){
	int i,j;
	for(i = 0; i < size; i++){
		for(j = 0; j < size; j++){
			if(i == j){
				printf("1 ");
			} else {
				printf("0 ");
			}
		}
		printf("\n");
	}
}

void printmatrix(int **matrix, int size){
	int i,j;
	for(i = 0; i < size; i++){
		for(j = 0; j < size; j++) {
			printf("%i ", matrix[i][j]);
		}
		printf("\n");
	}
}

int main(int argc, char** argv) {

	int size, power;

	if(!scanf("%i", &size)){
		return EXIT_FAILURE;
	}
	int **matrix = createMatrix(size);

	int i, j;

	for(i = 0; i < size; i++){
		for(j = 0; j < size; j++){
			if(!scanf("%i", &matrix[i][j])) {
				puts("error");
				return EXIT_FAILURE;
			}
		}
	}

	if(!scanf("%i", &power)){
		return EXIT_FAILURE;
	}

	if(power == 0){
		IMatrix(size);
	} else if(power < 0){
		return EXIT_FAILURE;
	} else if(!matrixmulti(matrix, power, size)){
		return EXIT_FAILURE;
	}

	return 0;
}
