/* PA2 estimate */

#include <stdio.h>
#include <stdlib.h>

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

/** Free a given Matrix.
 */
void FreeMatrix(struct Matrix A) {
    int i;
    for(i = 0; i < A.rows; ++i) {
        free(A.data[i]);
    }
}

/** Compare two strings against each other.
 */
int StringCompare(char *word, char *against) {
    int i = 0;
    while(word[i] != '\0') {
        if(word[i] != against[i]) {
            return 0;
        }
        ++i;
    }
    return 1;
}

/** Create a matrix given rows and cols.
 *  In our case our rows and cols will be either num of houses or num of attributes.
 */
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
void AssignMatrixSizes(FILE **trainingfile, int *attributes, int *numofhouses, struct Matrix *X, struct Matrix *Y) {
    if(fscanf(*trainingfile, "%i", attributes) == EOF){
        printf("error reading attributes\n");
        exit(EXIT_FAILURE);
    }

    if(fscanf(*trainingfile, "%i", numofhouses) == EOF){
        printf("error reading number of houses\n");
        exit(EXIT_FAILURE);
    }

    *X = CreateMatrix(*numofhouses, *attributes + 1);
    *Y = CreateMatrix(*numofhouses, 1);
}

/** Sort the files given in the arguments. This is useful because if the files are given
 * in any order the program can still run and know which file to use.
 */
char **sortFiles(char **file1, char **file2) {
    char **files = malloc(2 * sizeof(char*));
    files[0] = *file1;
    files[1] = *file2;
    int i, inputread, trainread;
    for(i = 0; i < 2; ++i){
        FILE *fp = fopen(files[i], "r");

        if(!fp){
            printf("error opening file\n");
            exit(EXIT_FAILURE);
        }

        char FileType[6];

        if(fscanf(fp, "%s",FileType) == EOF) {
            printf("error reading file type\n");
            exit(EXIT_FAILURE);
        }

        if(StringCompare("data", FileType)){
            inputread = 1;
            if(i == 1) break;
            files[0] = *file2;
            files[1] = *file1;
            i = -1;
        }

        if(StringCompare("train", FileType)){
            trainread = 1;
            if(i == 0) continue;
            files[0] = *file2;
            files[1] = *file1;
        }
        fclose(fp);
    }

    if(!inputread || !trainread){
        printf("Input Data file or Training File Not found!\n");
        exit(EXIT_FAILURE);
    }

    return files;
}

/** Print a given Matrix to stdout with decimal points.
 */
void PrintMatrix(struct Matrix Z) {
    int i,j;
    for(i = 0; i < Z.rows; ++i) {
        for(j = 0; j < Z.cols; ++j){
            printf("%lf ", Z.data[i][j]);
        }
        printf("\n");
    }
}

/** Print a given Matrix to stdout that has no decimal points.
 */
void PrintMatrixNoDec(struct Matrix Z) {
    int i,j;
    for(i = 0; i < Z.rows; ++i) {
        for(j = 0; j < Z.cols; ++j){
            printf("%.0f ", Z.data[i][j]);
        }
        printf("\n");
    }
}

/** Transpose any given Matrix. In terms of our Algorithm it will always transpose 
 * Matrix X.
 */
struct Matrix TransposeMatrix(struct Matrix X) {
    struct Matrix Trans = CreateMatrix(X.cols, X.rows);
    //Trans.data = CreateMatrix();
    int i, j;
    for(i = 0; i < X.rows; ++i) {
        for(j = 0; j < X.cols; ++j) {
            Trans.data[j][i] = X.data[i][j];
        }
    }
    return Trans;
}


/** Fill up Matrix X with the training data & fill Matrix Y with our House Prices. By the end Matrix X
 * should also only have 1's in the first column.
 */
void PopulateMatricies(FILE **fp, int attributes, int numofhouses, struct Matrix *X, struct Matrix *Y){
    int i, j;
    double value;
    for(i = 0; i < numofhouses; ++i) {
        // We are doing j < attr + 2 because the value of attributes passed in will be already one less
        // then the actual length of the file. So we increase by two so we can capture the last column.
        for(j = 0; j < (attributes + 2); ++j) {
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
 * This method is going to be our Gaussian Elimination Function.
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

/** Multiply any two Matricies together. Returns a new Result Matrix.
 */
struct Matrix MultiplyMatrix(struct Matrix X, struct Matrix Y) {
    int row, col, rrow, rcol;
    double result;
    struct Matrix product;
    if(X.cols != Y.rows) {
        printf("Unable to Multiply Matricies!\n");
        exit(EXIT_FAILURE);
    }
    product = CreateMatrix(X.rows, Y.cols);
    for(rrow = 0; rrow < X.rows; ++rrow) {
        for(rcol = 0; rcol < Y.cols; ++rcol) {
            result = col = 0;
            for(row = 0; row < Y.rows; ++row) {
                result = result + (X.data[rrow][col] * Y.data[row][rcol]);
                ++col;
            }
            product.data[rrow][rcol] = result;
        }
    }
    return product;
}

/** Predicts the price of houses given an input data file and a found Weight Matrix.
 * Prints the Prediciton to stdout.
 */
void Predict(FILE **InputData, struct Matrix W, int attributes, int numofhouses) {
    int i , j;
    double value, temp, one;
    printf("House Price Predictions:\n");
    for(i = 0; i < numofhouses; ++i) {
        value = W.data[0][0];
        for(j = 1; j < (attributes + 1); ++j) {
            one = fscanf(*InputData, "%lf", &temp);
            value = value + (W.data[j][0] * temp);
        }
        if(one) {
            printf("%0.0lf\n", value);
        }
    }
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
    // argv[1] & argv[2] contain the files for our training data and our input data.
    // Just in case they are given in different order, I implemented a function that will
    // Sort each file such that training data will always be in files[0] and the input data
    // will always be in files[1].
    char **files = sortFiles(&argv[1], &argv[2]);
    char *trainingdata = files[0];
    char *testdata = files[1];
    
    // Create all our neede dependicies for our algorithm to work.
    int attributes, numofhouses;
    struct Matrix X;
    struct Matrix XT;
    struct Matrix XTX;
    struct Matrix Y;
    struct Matrix InverXTX;
    struct Matrix InXTX_XT;
    struct Matrix W;

    // Begin opening the training data and going through the training data.
    FILE *fp = fopen(trainingdata, "r");
    char buffer[6];
    // Skip the first line of the file which just says "data" or "train".
    fscanf(fp, "%s", buffer);
    // Pass the file pointer to a function so the main function isn't so messy.
    // Look at the training data and assign Matrix sizes needed for each Matrix for function to work.
    AssignMatrixSizes(&fp, &attributes, &numofhouses, &X, &Y);
    // Go back through the training data and populate matrix X & Y with the relevant data needed in each
    // Matrix. X containing attributes & Y containing house prices.
    PopulateMatricies(&fp, attributes, numofhouses, &X, &Y);
    fclose(fp);

    XT = TransposeMatrix(X);
    XTX = MultiplyMatrix(XT, X);

    // InverXTX = Matrix Inverse (XTX)
    InXTX_XT = MultiplyMatrix(InverXTX, XT);

    W = MultiplyMatrix(InXTX_XT, Y);

    fp = fopen(testdata, "r");
    fscanf(fp, "%s", buffer);
    Predict(&fp, W, attributes, numofhouses);
    fclose(fp);
    // ... After all is said and done, we can return and enjoy the result.
    return 0;
}
