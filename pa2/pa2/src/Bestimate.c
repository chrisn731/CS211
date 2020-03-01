/* PA2 estimate */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * For This Project we are going to be using multiple matricies.
 * #############################################################################################################
 *     Matrix    | Purpose 
 * =============================================================================================================
 *      X        : Will Contain a column of 1's and the rest of them will contain the Training Data Attributes
 *  X Transposed : Will Contain the contents of our Matrix X but it will be transposed
 *    Inverse    : Will Contain the Inverse of the product of X Transposed and (((X^T)(X^-1))^-1)
 *      Y        : Will Contain the House Prices given to use by the Training Data File
 *  Inverse XT   : Will contain the resulting Matrix
 *      W        : Will contain our found weights
 * =============================================================================================================
 * Using these matricies we are going to computer W using the formula W = [ (( (X^T)(X^-1) )^-1) (X^T)(Y) ]
 * 
 * To calculate the inverse we will be using Gauss Jordan Elimination with some special things to note
 *      1) We will only be either be multiplying all the elements of the row by some value or adding the
 *          contents of one row to another
 *      2) We will not be swapping rows at any point within the algorithm. So in total only 3 operations
 *          will be performed
 * 
 * Now for that code...
 * #############################################################################################################
*/

// We are going to use a struct so we can easily keep track of each Matrix data and size
struct Matrix {
    double **data;
    int rows;
    int cols;
};

struct Matrix CreateMatrix(int rows, int cols) {
    struct Matrix new;
    new.cols = cols;
    new.rows = rows;
    new.data = malloc(sizeof(double*) * rows);

    if(!new.data){
        printf("error allocating memory to matrix");
        exit(EXIT_FAILURE);
    }

    int i;

    for(i = 0; i < rows; ++i) {
        new.data[i] = malloc(sizeof(double) * cols);
        if(!new.data[i]) {
            printf("error allocating memory to matrix\n");
            exit(EXIT_FAILURE);
        }
    }
    return new;
}

/**
 * Opens the training data file to get the sizes needed for our matrix to house the data.
 */
void AssignMatrixSizes(FILE **trainingfile, int *attributes, int *numofhouses, struct Matrix *X, struct Matrix *XT, struct Matrix *Y, struct Matrix *Inver, struct Matrix *InverXT, struct Matrix *W){
    if(fscanf(*trainingfile, "%i", attributes) == EOF){
        printf("error reading attributes\n");
        exit(EXIT_FAILURE);
    }

    if(fscanf(*trainingfile, "%i", numofhouses) == EOF){
        printf("error reading number of houses\n");
        exit(EXIT_FAILURE);
    }

    *X = CreateMatrix(*numofhouses, *attributes + 1);
    *XT = CreateMatrix(*attributes + 1, *numofhouses);
    *Y = CreateMatrix(*numofhouses, 1);
    *Inver = CreateMatrix(*attributes + 1, *numofhouses);
    *InverXT = CreateMatrix(*attributes + 1, *numofhouses);
    *W = CreateMatrix(*attributes + 1, 1);
}

char **sortFiles(char **file1, char **file2){
    char **files = malloc(2 * sizeof(char*));
    files[0] = *file1;
    files[1] = *file2;
    int i, inputread, trainread;
    for(i = 0; i < 2; ++i){
        FILE *fp;
        fp = fopen(files[i], "r");

        if(!fp){
            printf("error opening file\n");
            exit(EXIT_FAILURE);
        }

        char FileType[6];

        if(fscanf(fp, "%s",FileType) == EOF) {
            printf("error reading file type\n");
            exit(EXIT_FAILURE);
        }

        if(strcmp(FileType, "data")){
            inputread = 1;
            if(i == 1) continue;
            files[0] = *file2;
            files[1] = *file1;
            i = 0;
        }

        if(strcmp(FileType, "train")){
            trainread = 1;
            if(i == 0) continue;
            files[0] = *file2;
            files[1] = *file1;
        }
    }

    if(!inputread || !trainread){
        printf("Input Data file or Training File Not found!\n");
        exit(EXIT_FAILURE);
    }

    return files;
}

void PrintMatrix(struct Matrix Z){
    int i,j;
    for(i = 0; i < Z.rows; ++i) {
        for(j = 0; j < Z.cols; ++j){
            printf("%lf", Z.data[i][j]);
        }
        printf("\n");
    }
}

/*
double **TransposeMatrix(struct Matrix XT) {
    struct Matrix Trans;
    Trans.cols = XT.rows;
    Trans.rows = XT.cols;
    //Trans.data = CreateMatrix();
    int i, j;
}
*/

/** Fill up Matrix X with the training data & fill Matrix Y with our House Prices. By the end Matrix X
 * should also only have 1's in the first column.
 */
void PopulateMatricies(FILE **fp, int attributes, int numofhouses, struct Matrix *X, struct Matrix *Y){
    int i, j;
    double value;
    for(i = 0; i < numofhouses; ++i) {
        for(j = 0; j < (attributes + 1); ++j) {
            if(j == 0){
                (*X).data[i][j] = 1;
            } else {
                fscanf(*fp, "%lf", &value);
                if((j+1) == attributes){
                    (*Y).data[i][0] = value;
                } else {
                    (*X).data[i][j] = value;
                }
            }
        }
    }
}

/**
 * This method is going to be our Gaussian Elimination Function
 * Godspeed.
 */
double **InvertMatrix(struct Matrix Invert) {
    // We must construct a Identity Matrix 
    struct Matrix Iden = CreateMatrix(Invert.rows, Invert.cols);
    int i, j;

    for(i = 0; i < Iden.rows; ++i) {
        for(j = 0; i < Iden.cols; ++j) {
            if(i == j) {
                Iden.data[i][j] = 1;
            } else {
                Iden.data[i][j] = 0;
            }
        }
    }
    return 0;
}

int main(int argc, char **argv) {
    if(argc < 3){
        printf("error, not enough inputs\n");
        return 1;
    }

    if(argc > 3){
        printf("error, too many inputs\n");
        return 1;
    }
    char **files = sortFiles(&argv[1], &argv[2]);
    char *trainingdata = files[0];
    //char *testdata = files[1];
    
    int attributes, numofhouses;
    struct Matrix X;
    struct Matrix XT;
    struct Matrix Y;
    struct Matrix Inver;
    struct Matrix InverXT;
    struct Matrix W;

    FILE *fp = fopen(trainingdata, "r");
    AssignMatrixSizes(&fp, &attributes, &numofhouses, &X, &XT, &Y, &Inver, &InverXT, &W);
    PopulateMatricies(&fp, attributes, numofhouses, &X, &Y);
    PrintMatrix(X);
    fclose(fp);


    
    // argv[1] & argv[2] contain the files for our training data
    // and our input data. We will populate our matricies
    // using these two arguments.
    
    //PopulateMatricies(&TrainingData, &InputData, argv[1], argv[2]);

    // ... After all is said and done, we can free our heap

    return 0;
}
