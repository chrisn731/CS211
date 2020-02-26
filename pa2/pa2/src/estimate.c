/* PA2 estimate */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int TRAINFILEREAD = 0;

double **createMatrix(int attributes, int numofhouses){
    /**
     * This is necessary because for our Training Data because in our TrainingData matrix,
     * for N attributes we need N+1 columns. However this isnt the case with our InputData matrix,
     * Therefore once the TrainingData is Read we can reset it back to 0 so
     * this if never runs
     */
    if(TRAINFILEREAD) {
        attributes++;
        TRAINFILEREAD = 0;
    }
    double **matrix = malloc(sizeof(double*) * attributes);
    if(!matrix){
        printf("error allocating memory to matrix\n");
        exit(EXIT_FAILURE);
    }

    int i;
    // Now create the matrix rows for which they will be NumofHouses long
    for(i = 0; i < attributes; ++i){
        matrix[i] = malloc(sizeof(double) * numofhouses);
        if(!matrix[i]){
            printf("error allocating memory to matrix\n");
            exit(EXIT_FAILURE);
        }
    }
    // ... gracefully return
    return matrix;
}


double **readData(FILE *datafile){
    int attributes, numofhouses;
    if(fscanf(datafile, "%i", &attributes) == EOF){
        printf("error reading attributes\n");
        exit(EXIT_FAILURE);
    }

    if(fscanf(datafile, "%i", &numofhouses) == EOF){
        printf("error reading number of houses\n");
        exit(EXIT_FAILURE);
    }
    double **Data = createMatrix(attributes, numofhouses);
    int i ,j;
    // Iterate through the matrix given in the training data and then populate the
    // training data matrix
    for(i = 0; i < (attributes + 1); ++i) {
        for(j = 0; i < numofhouses; ++j) {
            fscanf(datafile, "%lf", &Data[i][j]);
        }
    }

    // Return our newly made Data Matrix for our AI
    return Data;
}

void PopulateMatricies(double ***TrainingDate, double ***InputData, char *file1, char *file2){
    int i, trainread = 0, inputread = 0;
    // Create an array to point to the two files so we can iterate through them
    char **files = malloc(2 * sizeof(char*));
    files[0] = file1;
    files[1] = file2;
    for(i = 0; i < 2; ++i){
        FILE *fp;
        fp = fopen(files[i], "r");

        if(!fp){
            printf("error reading file\n");
            exit(EXIT_FAILURE);
        }
        char *FileType = malloc(6 * sizeof(char));
        
        // Read the type of file, if it neither a training file or input file... exit the program
        if(fscanf(fp, "%s", FileType) == EOF) {
            printf("error reading file type\n");
            exit(EXIT_FAILURE);
        }

        // Next two if's will check to see what type of file it is.
        // Then assign the correct matrix to that file.
        if(strcmp(FileType, "data")){
            inputread = 1;
            *InputData = readData(fp);
        }

        if(strcmp(FileType, "train")){
            TRAINFILEREAD = 1;
            trainread = 1;
            *TrainingDate = readData(fp);
        }
		free(FileType);
    }

    // If either one of the files were not read then we can quit because
    // We are missing essential data for the program to work
    if(!trainread || !inputread){
        printf("error missing training data or input data\n");
        exit(EXIT_FAILURE);
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
    // This Matrix will be our Matrix 'X'
    double **TrainingData = NULL;
    double **InputData = NULL;
    PopulateMatricies(&TrainingData, &InputData, argv[0], argv[1]);


    free(TrainingData);
    free(InputData);

    return 0;
}
