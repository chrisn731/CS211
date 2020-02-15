#include <stdio.h>
#include <string.h>

int scanpalindrome(char* string, int strlen) {
	int i = 0;
	strlen--;
	char endchar = string[strlen];
	char beg = string[i];
	while(strlen > i) { 
		if( (beg >= 0 && beg <= 47) || (beg >= 58 && beg <= 64) || (beg >= 91 && beg <= 96) ) {
			i++;
			beg = string[i];
		} else if( (endchar >= 0 && endchar <= 47) || (endchar >= 58 && endchar <= 64) || (endchar >= 91 && endchar <= 96)) {
			strlen--;
			endchar = string[strlen];

		} else if (endchar >= 97 && endchar <= 122) {
			endchar -= 32;
		} else if (beg >= 97 && beg <= 122) {
			beg -= 32;
		} else if (beg != endchar) {
			return 0;
		} else {
			i++;
			strlen--;
			endchar = string[strlen];
			beg = string[i];
		}
	}
	return 1;
}

int main(int argc, char** argv) {
	if(argc == 1) return 0;
	char *palindrome = argv[1];
	int pallen = strlen(palindrome);

	if(scanpalindrome(palindrome, pallen)) {
		puts("yes");
		return 0;
	} else {
		puts("no");
		return 0;
	}
	
	return 1;

} 
