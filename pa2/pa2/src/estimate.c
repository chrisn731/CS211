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
 *     XTX       : Will contain the product of Matrix Multiplication of X and X transposed
 *   Inverse     : Will Contain the Inverse of the product of X Transposed and (((X^T)(X^-1))^-1)
 *      Y        : Will Contain the House Prices given by the Training Data File & The found house prices
 *  Inverse XT   : Will contain the product of matrix multiplication of Inverse and X Transposed
 *      W        : Will contain our found weights of the data given by the training data
 * =============================================================================================================
 * Using these matricies we are going to computer W using the formula W = [ (( (X^T)(X^-1) )^-1) (X^T)(Y) ]
 * Then we can create a new X matrix that holds our input data (our data to use for predictions)
 * Using our new X we can use the formula XW = Y to predict the house prices
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

/** Free a given Matrix. */
void FreeMatrix(struct Matrix A)
{
    int i;
    for(i = 0; i < A.rows; ++i)
        free(A.data[i]);

    free(A.data);
}

/** Compare two strings against each other. */
int StringCompare(char *p1, char *p2)
{
    char c1, c2;
    do {
		c1 = *p1++;
		c2 = *p2++;
		if(c1 == '\0')
			return c1- c2;
	} while (c1 == c2);

	return c1 - c2;
}

/** Create a matrix given rows and cols.
 *  In our case our rows and cols will be either number of houses or number of attributes. */
struct Matrix CreateMatrix(int rows, int cols)
{
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

/** Opens the training data file to get the sizes needed for our matrix to house the data.*/
void AssignMatrixSizes(FILE **trainingfile, int *attributes, int *numofhouses, struct Matrix *X, struct Matrix *Y)
{
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
 * in any order the program can still run and know which file to use. */
char **sortFiles(char **file1, char **file2)
{
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

        if(!StringCompare("data", FileType)){
            inputread = 1;
            if(i == 1)
		break;

            files[0] = *file2;
            files[1] = *file1;
            i = -1;
        }

        if(!StringCompare("train", FileType)){
            trainread = 1;
            if(i == 0)
		continue;

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

/** Print a given Matrix to stdout with decimal points. */
void PrintMatrix(struct Matrix Z)
{
    int i,j;
    for(i = 0; i < Z.rows; ++i) {
        for(j = 0; j < Z.cols; ++j)
            printf("%lf\t", Z.data[i][j]);

        printf("\n");
    }
}

/** Print a given Matrix to stdout that has no decimal points. */
void PrintMatrixNoDec(struct Matrix Z)
{
    int i,j;
    for(i = 0; i < Z.rows; ++i) {
        for(j = 0; j < Z.cols; ++j)
            printf("%.0f\n", Z.data[i][j]);
    }
}

/** Transpose any given Matrix. In terms of our Algorithm it will always transpose 
 * Matrix X. */
struct Matrix TransposeMatrix(struct Matrix X)
{
    struct Matrix Trans = CreateMatrix(X.cols, X.rows);
    int i, j;
    for(i = 0; i < X.rows; ++i) {
        for(j = 0; j < X.cols; ++j) 
            Trans.data[j][i] = X.data[i][j];
        
    }
    return Trans;
}


/** Fill up Matrix X with the training data & fill Matrix Y with our House Prices. By the end Matrix X
 * should also only have 1's in the first column.*/
void PopulateMatricies(FILE **fp, int attributes, int numofhouses, struct Matrix *X, struct Matrix *Y)
{
    int i, j;
    double value;
    for(i = 0; i < numofhouses; ++i) {
        // We are doing j < attr + 2 because the value of attributes passed in will be already one less
        // then the actual length of the file. So we increase by two so we can capture the last column.
        for(j = 0; j < (attributes + 2); ++j) {
            if(j == 0)
                X->data[i][j] = 1;
            else {
                fscanf(*fp, "%lf", &value);

                if((j+1) == (attributes+2))
                    Y->data[i][0] = value;
                else
                    X->data[i][j] = value;
            }
        }
    }
}

/** Full up a single Matrix with given data file. */
void PopulateMatrix(FILE **fp, int attributes, int numofhouses, struct Matrix *X)
{
    int i, j;
    double value;
    for(i = 0; i < numofhouses; ++i) {
        for(j = 0; j < (attributes + 1); ++j) {
            if(j == 0)
                X->data[i][j] = 1;

            else {
                fscanf(*fp, "%lf", &value);
                X->data[i][j] = value;
            }
        }
    }
}

/** This method is going to be our Gaussian Elimination Function. Godspeed. */
struct Matrix InvertMatrix(struct Matrix Invert)
{
    // Identity Matrix for holding our resulting Invert Matrix 
    struct Matrix Iden = CreateMatrix(Invert.rows, Invert.cols);
    int i, j;

    for(i = 0; i < Iden.rows; ++i) {
        for(j = 0; j < Iden.cols; ++j) 
            Iden.data[i][j] = (i == j) ? 1 : 0;

    }
    // Begin Gaussian Elimination

    // First Pass will get the matrix into REF
    for(i = 0; i < Invert.rows; ++i) {
        // If our pivot is not one, reduce the whole row so then it is one.
        if(Invert.data[i][i] != 1) {
           double reduce = Invert.data[i][i];
            for(j = 0; j < Invert.cols; ++j) {
                // Everything we do to our Original Matrix we must do to our identity.
               Invert.data[i][j] = Invert.data[i][j] / reduce;
               Iden.data[i][j] = Iden.data[i][j] / reduce;
            }
        }

        // We don't want to apply row operations beyond the final row.
        if((i+1) == Invert.rows) break;

        // Subtract other rows by current row to obtain our Reduced Rows.
        for(j = i + 1; j < Invert.rows; ++j) {
            // Get the subtraction multiplier of every value below our current pivot.
            double multi = Invert.data[j][i];
            int k;
            for(k = 0; k < Invert.cols; ++k) { 
                Invert.data[j][k] -= (multi * Invert.data[i][k]);
                Iden.data[j][k] -= (multi * Iden.data[i][k]);
            }
        }
    }

    // Second pass gets the Matrix into RREF.
    // All pivots will be one so no need to reduce.
    for(i = (Invert.rows - 1); i > 0; --i) {
        for(j = i - 1; j > -1; --j) {
            // Get the subtraction multiplier of every value above our current pivot.
            double multi = Invert.data[j][i];
            int k;
            for(k = (Invert.cols - 1); k > -1; --k) {
                Invert.data[j][k] -= (multi * Invert.data[i][k]);
                Iden.data[j][k] -= (multi * Iden.data[i][k]);
            }
        }
    }
    // End Gaussian Elimination

    // We will return the "Identity Matrix" because the operations performed on our "Invert"
    // Matrix will also apply to our identity Matrix. So our old identity matrix is now our new
    // Inverted Matrix. Which is what we want.
    return Iden;
}

/** Multiply any two Matricies together. Returns a new Result Matrix. */
struct Matrix MultiplyMatrix(struct Matrix X, struct Matrix Y)
{
    if(X.cols != Y.rows) {
        printf("Unable to Multiply Matricies!\n");
        exit(EXIT_FAILURE);
    }
    int row, col, rrow, rcol;
    double result;

    struct Matrix product = CreateMatrix(X.rows, Y.cols);
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

int main(int argc, char **argv)
{
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
    struct Matrix X, XT, XTX, Y, InverXTX, InXTX_XT, W;

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

    /* Easter Egg if youre a fan of one liners check this one line to get the Weight...
    W = MultiplyMatrix(MultiplyMatrix(InvertMatrix(MultiplyMatrix(TransposeMatrix(X),X)),XT),Y);
    ... Otherwise do it the normal way so its actually readable. */

    // Begin solving for W & free memory on the way...
    // First Transpose X to get X^T
    XT = TransposeMatrix(X);

    // Multiply X^T and X : X^TX
    XTX = MultiplyMatrix(XT, X);
    FreeMatrix(X);

    // Find the Inverse of that product : (X^T X)^-1
    InverXTX = InvertMatrix(XTX);
    FreeMatrix(XTX);

    // Multiply that inverse by the Transpose of X : ((X^T X)^-1)X^T
    InXTX_XT = MultiplyMatrix(InverXTX, XT);
    FreeMatrix(InverXTX);
    FreeMatrix(XT);

    // Final Multiplication. Multiply by Y to get W : (((X^T X)^-1)X^T)Y
    W = MultiplyMatrix(InXTX_XT, Y);
    FreeMatrix(InXTX_XT);
    FreeMatrix(Y);

    // Now that we have our weights, its time to predict the prices of houses with given input data.
    fp = fopen(testdata, "r");
    // Skip the first word of the file.
    fscanf(fp, "%s", buffer);

    // Obtain our new matrix X.
    fscanf(fp, "%d", &attributes);
    fscanf(fp, "%d", &numofhouses);

    X = CreateMatrix(numofhouses, attributes + 1);
    PopulateMatrix(&fp, attributes, numofhouses, &X);
    fclose(fp);

    // Obtain our new Y
    Y = MultiplyMatrix(X, W);
    PrintMatrixNoDec(Y);

    FreeMatrix(W);
    FreeMatrix(X);
    FreeMatrix(Y);
    free(files);
    
    // ... After all is said and done, we can return and enjoy the result.
    return 0;
}
