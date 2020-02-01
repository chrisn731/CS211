#include <stdio.h>
#include <stdlib.h>

FILE *fr;

int main() {


	int n;
	char line[80];

	fr = fopen ("test.txt" , "r");

	while (fgets(line, 80, fr) != NULL) {
		sscanf(line);	
	}	
	
	fclose(fr);
}

