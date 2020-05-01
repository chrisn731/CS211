#include <stdlib.h>
#include <stdio.h>

#DEFINE DEBUG 0

/* Calculates the result of log_2(x) */
int LogTwo(unsigned int num)
{
	if (num == 0)
		return 0;

	int result;

	while((num >>= 1) != 1)
		++result;

	return result;
}

/* Print out the results from the cache simulator. */
void CacheStats(int Prefetch, int MemR, 
				int MemW, int CacheH, int CacheM)
{

	printf("Prefetch %d\n", Prefetch);
	printf("Memory reads: %d\n", MemR);
	printf("Memory writes: %d\n", MemW);
	printf("Cache hits: %d\n", CacheH);
	printf("Cache misses: %d", CacheM);

}





int main(int argc, char *argv[])
{
	if (argc != 6) {
		puts("Usage: ./cachesim [cache size] [associativity] [replacement policy] [block size] [trace file]");
		return 1;
	}

	unsigned int cachesize, blocksize;
	char *associativity, *replacement, *tracefile;

	cachesize = atoi(argv[1]);
	blocksize = atoi(argv[4]);

	associativity = argv[2];
	replacement = argv[3];
	tracefile = argv[5];






	return 0;

}
