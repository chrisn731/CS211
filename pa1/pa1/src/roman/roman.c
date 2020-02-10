#include <stdio.h>
#include <stdlib.h>


void convert(int numtoconvert) {
	
	int Mcount, CMcount, Dcount, CDcount, Ccount, XCcount, Lcount, XLcount, Xcount, IXcount, Vcount, IVcount, Icount;
	Mcount = Dcount = Ccount = Lcount = Xcount = Vcount = Icount = CMcount = CDcount = XCcount = XLcount = IXcount = IVcount = 0;
	int i;
	
	if(numtoconvert >= 1000) {
		Mcount = numtoconvert / 1000;
		numtoconvert -= (Mcount * 1000);
		for(i=0; i < Mcount; i++) printf("M");	
	}

	if(numtoconvert >= 500) {
		if(numtoconvert / 900 == 1) {
			CMcount = 1;
			numtoconvert -= 900;
			printf("CM");
		} else {
			Dcount = numtoconvert / 500;
			numtoconvert -= (Dcount * 500);
			for(i = 0; i < Dcount; i++) printf("D");
		}
	}
	
	if(numtoconvert >= 100) {
		if(numtoconvert / 400 == 1) {
			CDcount = 1;
			numtoconvert -= 400;
			printf("CD");
		} else { 
			Ccount = numtoconvert / 100;
			numtoconvert -= (Ccount * 100);
			for(i = 0; i < Ccount; i++) printf("C");
		}
	}

	if(numtoconvert >= 50) {
		if(numtoconvert / 90 == 1) {
			XCcount = 1;
			numtoconvert -= 90;
			printf("XC");
		} else {
			Lcount = numtoconvert / 50;
			numtoconvert -= (Lcount * 50);
			for(i = 0; i < Lcount; i++) printf("L");
		}
	}
	
	if(numtoconvert >= 10) {
		if(numtoconvert / 40 == 1) {
			XLcount = 1;
			numtoconvert -= 40;
			printf("XL");
		} else {
			Xcount = numtoconvert / 10;
			numtoconvert -= (Xcount * 10);
			for(i = 0; i < Xcount; i++) printf("X");
		}
	}
	
	if(numtoconvert >= 5) {
		if(numtoconvert / 9 == 1) { 
			IXcount = 1;
			numtoconvert -= 9;
			printf("IX");
		} else { 
			Vcount = numtoconvert / 5;
			numtoconvert -= (Vcount * 5);
			for(i = 0; i < Vcount; i++) printf("V");
		}
	}

	if(numtoconvert >= 1) {
		if(numtoconvert / 4 == 1) {
			IVcount = 1;
			numtoconvert -= 4;		
			printf("IV");
		} else {
			Icount = numtoconvert / 1;
			numtoconvert -= (Icount);
			for(i = 0; i < Icount; i++) printf("I");
		}
	}
	printf("\n");
}


int main(int argc, char** argv) {
	if(argc == 0) return 0;
	int numtoconvert = atoi(argv[1]);
	if(numtoconvert == 0) return 0;
	convert(numtoconvert);	
}
