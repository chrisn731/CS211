#include <stdio.h>
#include <stdlib.h>

int compresslength(char* input) {

	char letter = input[0];
	int inputsize = 1;
	int i = 0;
	int lettercount = 1;
	int length = 0;
	while(letter != '\0') {

		if( (letter >= 47 && letter <= 57) ) return -1;

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


char *compress(char *output, char *input) {
	
	char currletter = input[0];
	char scanletter = input[0];
	int charcounter = 0;
	int inindex = 0;
	int outindex = 0;
	
	while(currletter != '\0') {

		if(currletter != scanletter) {
			output[outindex] = currletter;
			outindex++;
			sprintf((output+outindex),"%i",charcounter);
			outindex += 1 + (charcounter / 10);
			currletter = input[inindex];
			charcounter = 1;
		} else {
			charcounter++;
			inindex++;
		}
	}
	
	output[outindex] = '\0';
	return output;
	
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
		char *output = (char*)malloc(length * sizeof(char));
		output = compress(output, input);
		puts(output);	
		return 0;
	}
	
	return 1;
}
