#include <stdio.h>
#include <stdlib.h>

int compresslength(char* input) {

	char letter = input[0];
	int inputsize = 0;
	int i = 0;
	int lettercount = 1;
	int length = 0;
	while(letter != '\0') {

		if( (letter >= 48 && letter <= 57) ) return -1;

		if( (letter != input[i]) ) {
			//Every part of the output string is going to have (letter)(number). If the letter count > 10, use the / 10 to increase length of output string.
			length += 2 + (lettercount / 10);
			lettercount = 1;
			letter = input[i];
		} else {
			i++;
			lettercount++;
			inputsize++;
		}
	}
	if(length > inputsize) return 0;
	return length;
}


int compress(char *input) {
	
	char currletter = input[0];
	char scanletter = input[0];
	int charcounter = 0;
	int inindex = 0;
	
	while(scanletter != '\0') {
		if(scanletter != currletter) {
			printf("%c%i",currletter,charcounter);
			currletter = scanletter;
			charcounter = 0;	
		} else {
			inindex++;
			charcounter++;
			scanletter = input[inindex];

		}
	}
	printf("%c%i\n",currletter, charcounter);
	
	return 0;
}



int main(int argc, char** argv) {
	if(argc == 1) return 0;

	char* input = argv[1];
	int length = compresslength(input);

	if(length == -1) {
		puts("error");
		return 0;
	} else if (length == 0) {
		puts(input);
		return 0;
	} else {
		compress(input);
		return 0;
	}
	
	return 1;
}
