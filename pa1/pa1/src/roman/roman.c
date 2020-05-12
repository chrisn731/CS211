#include <stdio.h>
#include <stdlib.h>

void convert(int numtoconvert)
{

	int i, counter = 0;
	
	if(numtoconvert >= 1000) {
		counter = numtoconvert / 1000;
		numtoconvert -= (counter * 1000);
		for(i=0; i < counter; i++)
			printf("M");	
	}

	if(numtoconvert >= 500) {
		if(numtoconvert / 900 == 1) {
			numtoconvert -= 900;
			printf("CM");
		}
		else {
			counter = numtoconvert / 500;
			numtoconvert -= (counter * 500);
			for(i = 0; i < counter; i++)
				printf("D");
		}
	}
	
	if(numtoconvert >= 100) {
		if(numtoconvert / 400 == 1) {
			numtoconvert -= 400;
			printf("CD");
		}
		else { 
			counter = numtoconvert / 100;
			numtoconvert -= (counter * 100);
			for(i = 0; i < counter; i++)
				printf("C");
		}
	}

	if(numtoconvert >= 50) {
		if(numtoconvert / 90 == 1) {
			numtoconvert -= 90;
			printf("XC");
		}
		else {
			counter = numtoconvert / 50;
			numtoconvert -= (counter * 50);
			for(i = 0; i < counter; i++)
				printf("L");
		}
	}
	
	if(numtoconvert >= 10) {
		if(numtoconvert / 40 == 1) {
			numtoconvert -= 40;
			printf("XL");
		}
		else {
			counter = numtoconvert / 10;
			numtoconvert -= (counter * 10);
			for(i = 0; i < counter; i++)
				printf("X");
		}
	}
	
	if(numtoconvert >= 5) {
		if(numtoconvert / 9 == 1) { 
			numtoconvert -= 9;
			printf("IX");
		}
		else { 
			counter = numtoconvert / 5;
			numtoconvert -= (counter * 5);
			for(i = 0; i < counter; i++)
				printf("V");
		}
	}

	if(numtoconvert >= 1) {
		if(numtoconvert / 4 == 1) {
			numtoconvert -= 4;		
			printf("IV");
		}
		else {
			counter = numtoconvert / 1;
			numtoconvert -= (counter);
			for(i = 0; i < counter; i++)
				printf("I");
		}
	}
	printf("\n");
}


int main(int argc, char **argv)
{
	if(argc == 0) 
		return 1;
	
	int numtoconvert = atoi(argv[1]);

	if(numtoconvert == 0)
		return 0;

	convert(numtoconvert);	
	return 0;
}
