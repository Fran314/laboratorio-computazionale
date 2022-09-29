#include "palisade.h"

// header files needed for serialization
#include "ciphertext-ser.h"
#include "cryptocontext-ser.h"
#include "pubkeylp-ser.h"
#include "scheme/bfvrns/bfvrns-ser.h"

using namespace lbcrypto;

const std::string DATAFOLDER = "demoData";

int main(int argc, char *argv[])
{
	std::cout << "This program requres the subdirectory `" << DATAFOLDER
			  << "' to exist, otherwise you will get "
			  << "an error writing serializations." << std::endl;

	// Set the main parameters
	int plaintextModulus = 65537;
	double sigma = 3.2;
	SecurityLevel securityLevel = HEStd_256_classic;
	uint32_t depth = 2;

	// Sample Program: Step 1: Set CryptoContext
	// Instantiate the crypto context
	CryptoContext<DCRTPoly> cryptoContext =
		CryptoContextFactory<DCRTPoly>::genCryptoContextBFVrns(
			plaintextModulus, securityLevel, sigma, 0, depth, 0, OPTIMIZED);

	// Enable features that you wish to use
	cryptoContext->Enable(ENCRYPTION);
	cryptoContext->Enable(SHE);

	// Generate a public/private key pair
	LPKeyPair<DCRTPoly> keyPair = cryptoContext->KeyGen();

	// Serialize cryptocontext
	if (!Serial::SerializeToFile(DATAFOLDER + "/cryptocontext.txt", cryptoContext,
								 SerType::BINARY))
	{
		std::cerr << "Error writing serialization of the crypto context to "
					 "cryptocontext.txt"
				  << std::endl;
		return 1;
	}
	// Serialize the public key
	if (!Serial::SerializeToFile(DATAFOLDER + "/key-public.txt",
								 keyPair.publicKey, SerType::BINARY))
	{
		std::cerr << "Error writing serialization of public key to key-public.txt"
				  << std::endl;
		return 1;
	}
	std::cout << "The public key has been serialized." << std::endl;
	// Serialize the secret key
	if (!Serial::SerializeToFile(DATAFOLDER + "/key-private.txt",
								 keyPair.secretKey, SerType::BINARY))
	{
		std::cerr << "Error writing serialization of private key to key-private.txt"
				  << std::endl;
		return 1;
	}
	std::cout << "The secret key has been serialized." << std::endl;

	// Sample Program: Step 3: Encryption
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

	if (!Serial::SerializeToFile(DATAFOLDER + "/" + "ciphertext1.txt",
								 ciphertext1, SerType::BINARY))
	{
		std::cerr
			<< "Error writing serialization of ciphertext 1 to ciphertext1.txt"
			<< std::endl;
		return 1;
	}
	if (!Serial::SerializeToFile(DATAFOLDER + "/" + "ciphertext2.txt",
								 ciphertext2, SerType::BINARY))
	{
		std::cerr
			<< "Error writing serialization of ciphertext 2 to ciphertext2.txt"
			<< std::endl;
		return 1;
	}
	if (!Serial::SerializeToFile(DATAFOLDER + "/" + "ciphertext3.txt",
								 ciphertext3, SerType::BINARY))
	{
		std::cerr
			<< "Error writing serialization of ciphertext 3 to ciphertext3.txt"
			<< std::endl;
		return 1;
	}
	std::chrono::steady_clock::time_point data_ser_end = std::chrono::steady_clock::now();

	// Sample Program: Step 4: Evaluation

	// PALISADE maintains an internal map of CryptoContext objects which are
	// indexed by a tag and the tag is applied to both the CryptoContext and some
	// of the keys. When deserializing a context, PALISADE checks for the tag and
	// if it finds it in the CryptoContext map, it will return the stored version.
	// Hence, we need to clear the context and clear the keys.
	cryptoContext->ClearEvalMultKeys();
	cryptoContext->ClearEvalAutomorphismKeys();
	lbcrypto::CryptoContextFactory<lbcrypto::DCRTPoly>::ReleaseAllContexts();

	// Deserialize the crypto context
	CryptoContext<DCRTPoly> cc;
	if (!Serial::DeserializeFromFile(DATAFOLDER + "/cryptocontext.txt", cc,
									 SerType::BINARY))
	{
		std::cerr << "I cannot read serialization from "
				  << DATAFOLDER + "/cryptocontext.txt" << std::endl;
		return 1;
	}
	LPPublicKey<DCRTPoly> pk;
	if (Serial::DeserializeFromFile(DATAFOLDER + "/key-public.txt", pk,
									SerType::BINARY) == false)
	{
		std::cerr << "Could not read public key" << std::endl;
		return 1;
	}
	LPPrivateKey<DCRTPoly> sk;
	if (Serial::DeserializeFromFile(DATAFOLDER + "/key-private.txt", sk,
									SerType::BINARY) == false)
	{
		std::cerr << "Could not read secret key" << std::endl;
		return 1;
	}
	std::ifstream emkeys(DATAFOLDER + "/key-eval-mult.txt",
						 std::ios::in | std::ios::binary);
	Ciphertext<DCRTPoly> ct1;
	if (Serial::DeserializeFromFile(DATAFOLDER + "/ciphertext1.txt", ct1,
									SerType::BINARY) == false)
	{
		std::cerr << "Could not read the ciphertext" << std::endl;
		return 1;
	}
	Ciphertext<DCRTPoly> ct2;
	if (Serial::DeserializeFromFile(DATAFOLDER + "/ciphertext2.txt", ct2,
									SerType::BINARY) == false)
	{
		std::cerr << "Could not read the ciphertext" << std::endl;
		return 1;
	}
	Ciphertext<DCRTPoly> ct3;
	if (Serial::DeserializeFromFile(DATAFOLDER + "/ciphertext3.txt", ct3,
									SerType::BINARY) == false)
	{
		std::cerr << "Could not read the ciphertext" << std::endl;
		return 1;
	}
	std::chrono::steady_clock::time_point data_de_end = std::chrono::steady_clock::now();

	std::chrono::steady_clock::time_point hom_begin = std::chrono::steady_clock::now();
	// Homomorphic additions
	Plaintext plaintextAddResult;
	auto ciphertextAdd12 = cc->EvalAdd(ct1, ct2);
	auto ciphertextAddResult = cc->EvalAdd(ciphertextAdd12, ct3);
	cc->Decrypt(sk, ciphertextAddResult, &plaintextAddResult);

	// Output results
	std::cout << "Plaintext #1: " << plaintext1 << std::endl;
	std::cout << "Plaintext #2: " << plaintext2 << std::endl;
	std::cout << "Plaintext #3: " << plaintext3 << std::endl;

	std::cout << "Results of homomorphic computations" << std::endl;
	std::cout << "#1 + #2 + #3: " << plaintextAddResult << std::endl;
	return 0;
}
