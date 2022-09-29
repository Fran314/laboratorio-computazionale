#include "palisade.h"

#include <stdlib.h>
#include <time.h>
#include <chrono>

using namespace lbcrypto;

int main(int argc, char *argv[])
{
	srand(time(NULL));

	int LEV = (argc > 1) ? std::stoi(argv[1]) : 1;

	SecurityLevel securityLevel = (LEV == 1)   ? HEStd_256_classic
								  : (LEV == 2) ? HEStd_192_classic
											   : HEStd_128_classic;

	int SAMPLE_SIZE = (argc > 2) ? std::stoi(argv[2]) : 1000;
	int VALUES_AMOUNT = (argc > 3) ? std::stoi(argv[3]) : 8192;

	long long int total = 0;
	for (int iter = 0; iter < SAMPLE_SIZE; iter++)
	{
		// Set the main parameters
		int plaintextModulus = 65537;
		double sigma = 3.2;
		uint32_t depth = 2;

		// Instantiate the crypto context
		CryptoContext<DCRTPoly> cryptoContext =
			CryptoContextFactory<DCRTPoly>::genCryptoContextBFVrns(
				plaintextModulus, securityLevel, sigma, 0, depth, 0, OPTIMIZED);
		cryptoContext->Enable(ENCRYPTION);
		cryptoContext->Enable(SHE);

		LPKeyPair<DCRTPoly> keyPair = cryptoContext->KeyGen();

		// Generate Data
		std::vector<int64_t> vectorOfInts;
		for (int i = 0; i < VALUES_AMOUNT; i++)
		{
			vectorOfInts.push_back(rand() % plaintextModulus);
		}

		// Encrypt
		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
		Plaintext plaintext = cryptoContext->MakePackedPlaintext(vectorOfInts);
		auto ciphertext = cryptoContext->Encrypt(keyPair.publicKey, plaintext);
		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

		total += std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
	}

	std::cout << total << std::endl;

	return 0;
}
