#include <stdlib.h>
#include <stdio.h>


void filetest(FILE *fp) {
	char buffer[6];
	fscanf(fp, "%s", buffer);
	puts(buffer);
	
}


int main(int argc, char **argv) {
	FILE *fp;

	fp = fopen(argv[1], "r");
	if(!fp) {
		return 1;
	}
	filetest(fp);
	return 0;
}
