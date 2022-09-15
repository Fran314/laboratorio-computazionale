#include "palisade.h"
#include <chrono>

using namespace lbcrypto;

int main(int argc, char *argv[])
{
	int FLAGS = (argc > 1) ? std::stoi(argv[1]) : 7;
	bool ADD = FLAGS & 0x1, MULT = FLAGS & 0x2, ROT = FLAGS & 0x4;

	int SAMPLE_SIZE = (argc > 2) ? std::stoi(argv[2]) : 7;

	// Set the main parameters
	int plaintextModulus = 65537;
	double sigma = 3.2;
	SecurityLevel securityLevel = HEStd_128_classic;
	uint32_t depth = 2;

	// Sample Program: Step 1 - Set CryptoContext
	std::chrono::steady_clock::time_point tot_begin = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point cc_gen_begin = std::chrono::steady_clock::now();
	// Instantiate the crypto context
	CryptoContext<DCRTPoly> cryptoContext =
		CryptoContextFactory<DCRTPoly>::genCryptoContextBFVrns(
			plaintextModulus, securityLevel, sigma, 0, depth, 0, OPTIMIZED);

	// Enable features that you wish to use
	cryptoContext->Enable(ENCRYPTION);
	cryptoContext->Enable(SHE);

	// Generate a public/private key pair
	LPKeyPair<DCRTPoly> keyPair = cryptoContext->KeyGen();

	if (MULT)
	{
		// Generate the relinearization key
		cryptoContext->EvalMultKeyGen(keyPair.secretKey);
	}
	if (ROT)
	{
		// Generate the rotation evaluation keys
		cryptoContext->EvalAtIndexKeyGen(keyPair.secretKey, {1, 2, -1, -2});
	}
	std::chrono::steady_clock::time_point cc_gen_end = std::chrono::steady_clock::now();

	// First plaintext vector is encoded
	std::vector<int64_t> vectorOfInts1 = {1, 2, 3, 4, 5};
	std::vector<int64_t> vectorOfInts2 = {3, 2, 1, 4, 5};
	std::vector<int64_t> vectorOfInts3 = {1, 2, 5, 2, 5};
	for (int i = 6; i < SAMPLE_SIZE; i++)
	{
		vectorOfInts1.push_back(i);
		vectorOfInts2.push_back(i);
		vectorOfInts3.push_back(i);
	}
	std::chrono::steady_clock::time_point data_enc_begin = std::chrono::steady_clock::now();
	Plaintext plaintext1 = cryptoContext->MakePackedPlaintext(vectorOfInts1);
	Plaintext plaintext2 = cryptoContext->MakePackedPlaintext(vectorOfInts2);
	Plaintext plaintext3 = cryptoContext->MakePackedPlaintext(vectorOfInts3);

	// The encoded vectors are encrypted
	auto ciphertext1 = cryptoContext->Encrypt(keyPair.publicKey, plaintext1);
	auto ciphertext2 = cryptoContext->Encrypt(keyPair.publicKey, plaintext2);
	auto ciphertext3 = cryptoContext->Encrypt(keyPair.publicKey, plaintext3);
	std::chrono::steady_clock::time_point data_enc_end = std::chrono::steady_clock::now();

	std::chrono::steady_clock::time_point hom_begin = std::chrono::steady_clock::now();
	// Homomorphic additions
	Plaintext plaintextAddResult;
	if (ADD)
	{
		auto ciphertextAdd12 = cryptoContext->EvalAdd(ciphertext1, ciphertext2);
		auto ciphertextAddResult = cryptoContext->EvalAdd(ciphertextAdd12, ciphertext3);
		cryptoContext->Decrypt(keyPair.secretKey, ciphertextAddResult, &plaintextAddResult);
	}

	// Homomorphic multiplications
	Plaintext plaintextMultResult;
	if (MULT)
	{
		auto ciphertextMul12 = cryptoContext->EvalMult(ciphertext1, ciphertext2);
		auto ciphertextMultResult = cryptoContext->EvalMult(ciphertextMul12, ciphertext3);
		cryptoContext->Decrypt(keyPair.secretKey, ciphertextMultResult, &plaintextMultResult);
	}

	// Homomorphic rotations
	Plaintext plaintextRot1;
	Plaintext plaintextRot2;
	Plaintext plaintextRot3;
	Plaintext plaintextRot4;
	if (ROT)
	{
		auto ciphertextRot1 = cryptoContext->EvalAtIndex(ciphertext1, 1);
		auto ciphertextRot2 = cryptoContext->EvalAtIndex(ciphertext1, 2);
		auto ciphertextRot3 = cryptoContext->EvalAtIndex(ciphertext1, -1);
		auto ciphertextRot4 = cryptoContext->EvalAtIndex(ciphertext1, -2);
		cryptoContext->Decrypt(keyPair.secretKey, ciphertextRot1, &plaintextRot1);
		cryptoContext->Decrypt(keyPair.secretKey, ciphertextRot2, &plaintextRot2);
		cryptoContext->Decrypt(keyPair.secretKey, ciphertextRot3, &plaintextRot3);
		cryptoContext->Decrypt(keyPair.secretKey, ciphertextRot4, &plaintextRot4);
		plaintextRot1->SetLength(vectorOfInts1.size());
		plaintextRot2->SetLength(vectorOfInts1.size());
		plaintextRot3->SetLength(vectorOfInts1.size());
		plaintextRot4->SetLength(vectorOfInts1.size());
	}
	std::chrono::steady_clock::time_point hom_end = std::chrono::steady_clock::now();

	std::chrono::steady_clock::time_point tot_end = std::chrono::steady_clock::now();

	// Output results
	std::cout << "Plaintext #1: " << plaintext1 << std::endl;
	std::cout << "Plaintext #2: " << plaintext2 << std::endl;
	std::cout << "Plaintext #3: " << plaintext3 << std::endl;

	std::cout << "\nResults of homomorphic computations" << std::endl;
	if (ADD)
	{
		std::cout << "#1 + #2 + #3: " << plaintextAddResult << std::endl;
	}
	if (MULT)
	{
		std::cout << "#1 * #2 * #3: " << plaintextMultResult << std::endl;
	}
	if (ROT)
	{
		std::cout << "Left rotation of #1 by 1: " << plaintextRot1 << std::endl;
		std::cout << "Left rotation of #1 by 2: " << plaintextRot2 << std::endl;
		std::cout << "Right rotation of #1 by 1: " << plaintextRot3 << std::endl;
		std::cout << "Right rotation of #1 by 2: " << plaintextRot4 << std::endl;
	}
	std::cout << std::endl;

	std::cout << "Cipher context generation: " << std::chrono::duration_cast<std::chrono::microseconds>(cc_gen_end - cc_gen_begin).count() << "[µs]" << std::endl;
	std::cout << "Data encryption: " << std::chrono::duration_cast<std::chrono::microseconds>(data_enc_end - data_enc_begin).count() << "[µs]" << std::endl;
	std::cout << "Operations and decryption: " << std::chrono::duration_cast<std::chrono::microseconds>(hom_end - hom_begin).count() << "[µs]" << std::endl;
	std::cout << "Total: " << std::chrono::duration_cast<std::chrono::microseconds>(tot_end - tot_begin).count() << "[µs]" << std::endl;
	return 0;
}
