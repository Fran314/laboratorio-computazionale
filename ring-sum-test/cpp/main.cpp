#include "crypto-context.h"
#include "utils.h"

#include <stdlib.h>
#include <time.h>
#include <iostream>

int main(int argc, char *argv[])
{
	srand(time(NULL));

	// Setup
	int N = (argc > 1) ? std::stoi(argv[1]) : 4;

	// "Key exchange"
	int keys[N];
	for (int i = 0; i < N; i++)
	{
		keys[i] = rand();
	}
	CypherContext ctx[N];
	for (int i = 0; i < N; i++)
	{
		ctx[i].setPrimitives(sha256, sha256, hmac_sha256);
		ctx[i].setKeys(keys[i], keys[(i + 1) % N]);
	}

	// Individual plaintext and ciphertext generation
	int t = 0;
	int vals[N], cfrs[N];
	for (int i = 0; i < N; i++)
	{
		vals[i] = rand() % 0x10000;
		cfrs[i] = ctx[i].cypher(vals[i], t);
	}

	// Aggregation
	int clear_sum = 0, cipher_sum = 0;
	for (int i = 0; i < N; i++)
	{
		clear_sum += vals[i];
		cipher_sum += cfrs[i];
	}

	// Output
	print_results(N, vals, cfrs, clear_sum, cipher_sum); // from "utils.h"

	return 0;
}