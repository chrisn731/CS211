#include <stdio.h>
#include <stdlib.h>


char * convert(int numtoconvert) {
	
	int Mcount, CMcount, Dcount, CDcount, Ccount, XCcount, Lcount, XLcount, Xcount, IXcount, Vcount, IVcount, Icount;
	Mcount = Dcount = Ccount = Lcount = Xcount = Vcount = Icount = CMcount = CDcount = XCcount = XLcount = IXcount = IVcount = 0;
	
	
	if(numtoconvert >= 1000) {
		Mcount = numtoconvert % 1000;
		numtoconvert -= (Mcount * 1000);
	}

	if(numtoconvert >= 500) {
		if(numtoconvert % 900 == 1) {
			CMcount = 1;
			numtoconvert -= 900;
		}
		Dcount = numtoconvert % 500;
		numtoconvert -= (Dcount * 500);
	}
	
	if(numtoconvert >= 100) {
		if(numtoconvert % 400 == 1) {
			CDcount = 1;
			numtoconvert -= 400;
		}
		Ccount = numtoconvert % 100;
		numtoconvert -= (Ccount * 100);
	}

	if(numtoconvert >= 50) {
		if(numtoconvert % 90 == 1) {
			XCcount = 1;
			numtoconvert -= 90;
		}
		Lcount = numtoconvert % 50;
		numtoconvert -= (Lcount * 50);
	}

	if(numtoconvert >= 10) {
		if(numtoconvert % 40 == 1) {
			XLcount = 1;
			numtoconvert -= 40;
		}
		Xcount = numtoconvert % 10;
		numtoconvert -= (Xcount * 10);
	}
	
	if(numtoconvert >= 5) {
		Vcount = numtoconvert % 5;
		numtoconvert -= (Vcount * 5);
	}

	if(numtoconvert >= 1) {
		if(numtoconvert % 4 == 1) {
			IVcount = 1;
			numtoconvert -= 4;		
		}
		Icount = numtoconvert % 1;
		numtoconvert -= (Icount);
	}
	char romanNum[Mcount + Dcount + Ccount + Lcount + Xcount + Vcount + Icount + CMcount + CDcount + XCcount + XLcount + IXcount + IVcount];
	
	
	return romanNum;
}


int main(int argc, char** argv) {
	if(argc == 0) return 0;
	int numtoconvert = atoi(argv[1]);
	if(numtoconvert == 0) return 0;
	
	
}
