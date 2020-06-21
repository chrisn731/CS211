#include <stdlib.h>
#include <stdio.h>

#define DEBUG 0

void die(const char *err)
{
	fprintf(stderr, err);
	fputc('\n', stderr);
	exit(1);
}

/* Calculates the result of log_2(x) */
int LogTwo(unsigned int num)
{
	int result;

	if (num == 0)
		return 0;

	while((num >>= 1) != 1)
		++result;

	return result;
}

/* Print out the results from the cache simulator. */
void CacheStats(int Prefetch, int MemR, int MemW, int CacheH, int CacheM)
{

	printf("Prefetch %d\n", Prefetch);
	printf("Memory reads: %d\n", MemR);
	printf("Memory writes: %d\n", MemW);
	printf("Cache hits: %d\n", CacheH);
	printf("Cache misses: %d\n", CacheM);

}





int main(int argc, char *argv[])
{
	unsigned int cachesize, blocksize;
	char *associativity, *replacement, *tracefile;

	if (argc != 6) {
		die("Usage: ./cachesim [cache size] [associativity] [replacement policy] [block size] [trace file]");
		return 1;
	}

	cachesize = atoi(argv[1]);
	blocksize = atoi(argv[4]);

	associativity = argv[2];
	replacement = argv[3];
	tracefile = argv[5];






	return 0;

}
