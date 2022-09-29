#include "palisade.h"

using namespace lbcrypto;

int main(int argc, char *argv[])
{
	// Set the main parameters
	int plaintextModulus = 65537;
	double sigma = 3.2;
	SecurityLevel securityLevel = HEStd_256_classic;
	uint32_t depth = 2;

	// Sample Program: Step 1 - Set CryptoContext
	// Instantiate the crypto context
	CryptoContext<DCRTPoly> cryptoContext =
		CryptoContextFactory<DCRTPoly>::genCryptoContextBFVrns(
			plaintextModulus, securityLevel, sigma, 0, depth, 0, OPTIMIZED);

	// Enable features that you wish to use
	cryptoContext->Enable(ENCRYPTION);
	cryptoContext->Enable(SHE);

	// Generate a public/private key pair
	LPKeyPair<DCRTPoly> keyPair = cryptoContext->KeyGen();

	// First plaintext vector is encoded
	std::vector<int64_t> vectorOfInts1 = {1, 2, 3, 4, 5, 6, 7, 8};
	std::vector<int64_t> vectorOfInts2 = {3, 2, 1, 4, 5, 6, 7, 8};
	std::vector<int64_t> vectorOfInts3 = {1, 2, 5, 2, 5, 6, 7, 8};

	Plaintext plaintext1 = cryptoContext->MakePackedPlaintext(vectorOfInts1);
	Plaintext plaintext2 = cryptoContext->MakePackedPlaintext(vectorOfInts2);
	Plaintext plaintext3 = cryptoContext->MakePackedPlaintext(vectorOfInts3);

	// The encoded vectors are encrypted
	auto ciphertext1 = cryptoContext->Encrypt(keyPair.publicKey, plaintext1);
	auto ciphertext2 = cryptoContext->Encrypt(keyPair.publicKey, plaintext2);
	auto ciphertext3 = cryptoContext->Encrypt(keyPair.publicKey, plaintext3);

	// Homomorphic additions
	Plaintext plaintextAddResult;
	auto ciphertextAdd12 = cryptoContext->EvalAdd(ciphertext1, ciphertext2);
	auto ciphertextAddResult = cryptoContext->EvalAdd(ciphertextAdd12, ciphertext3);
	cryptoContext->Decrypt(keyPair.secretKey, ciphertextAddResult, &plaintextAddResult);

	// Output results
	std::cout << "Plaintext #1: " << plaintext1 << std::endl;
	std::cout << "Plaintext #2: " << plaintext2 << std::endl;
	std::cout << "Plaintext #3: " << plaintext3 << std::endl;

	std::cout << "Results of homomorphic computations" << std::endl;
	std::cout << "#1 + #2 + #3: " << plaintextAddResult << std::endl;
	return 0;
}
