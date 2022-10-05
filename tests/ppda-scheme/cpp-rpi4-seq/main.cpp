#include "crypto-context.h"

#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <chrono>

int main(int argc, char *argv[])
{
	int SAMPLE_SIZE = (argc > 1) ? std::stoi(argv[1]) : 1000;
	int VALUES_AMOUNT = (argc > 2) ? std::stoi(argv[2]) : 8192;

	srand(time(NULL));

	long long int total = 0;
	for (int iter = 0; iter < SAMPLE_SIZE; iter++)
	{
		CypherContext ctx;
		ctx.setKeys(rand(), rand());

		// Data generation
		int vals[VALUES_AMOUNT], cfrs[VALUES_AMOUNT];
		for (int i = 0; i < VALUES_AMOUNT; i++)
		{
			vals[i] = rand() % 0x10000;
		}
		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
		for (int i = 0; i < VALUES_AMOUNT; i++)
		{
			cfrs[i] = ctx.cypher(vals[i], i);
		}
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		total += std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
	}

	std::cout << total << std::endl;

	return 0;
}