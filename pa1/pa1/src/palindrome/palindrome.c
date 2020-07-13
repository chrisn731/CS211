#include <stdio.h>
#include <string.h>

int IsNotALetter(char c)
{
	return ((c >= 0 && c <= 47) || (c >= 58 && c <= 64) || (c >= 91 && c <= 96));
}

int scan(char *string, int strlen)
{
	char *endstring = &(string[strlen]);
	char begin, end;
	int halfway = strlen / 2;
	do {
		// Iterate through until you find a letter
		do {
			begin = *string++;
		} while (IsNotALetter(begin));

		// Same idea as above
		do {
			end = *endstring--;
		} while (IsNotALetter(end));

		// Keep the letters lowercase please :)
		if (end >= 97 && end <= 122)
			end -= 32;

		// Same as above :)
		if (begin >= 97 && begin <= 122)
			begin -= 32;

		if (begin != end)
			return 0;

	} while (strlen-- > halfway);

	return 1;
}


int main(int argc, char **argv)
{
	char *palindrome = argv[1];
	int strlength = strlen(palindrome);

	if(argc == 1 || argc > 2)
		return 1;

	if (scan(palindrome, strlength))
		puts("yes");

	else
		puts("no");

	return 0;
}
