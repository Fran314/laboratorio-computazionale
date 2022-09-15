#include "palisade.h"

// header files needed for serialization
#include "ciphertext-ser.h"
#include "cryptocontext-ser.h"
#include "pubkeylp-ser.h"
#include "scheme/bfvrns/bfvrns-ser.h"

#include <chrono>

using namespace lbcrypto;

const std::string DATAFOLDER = "demoData";

int main(int argc, char *argv[])
{
	std::cout << "This program requres the subdirectory `" << DATAFOLDER
			  << "' to exist, otherwise you will get "
			  << "an error writing serializations." << std::endl;

	int FLAGS = (argc > 1) ? std::stoi(argv[1]) : 7;
	bool ADD = FLAGS & 0x1, MULT = FLAGS & 0x2, ROT = FLAGS & 0x4;

	int SAMPLE_SIZE = (argc > 2) ? std::stoi(argv[2]) : 7;

	// Set the main parameters
	int plaintextModulus = 65537;
	double sigma = 3.2;
	SecurityLevel securityLevel = HEStd_128_classic;
	uint32_t depth = 2;

	// Sample Program: Step 1: Set CryptoContext
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

	std::chrono::steady_clock::time_point cc_ser_begin = std::chrono::steady_clock::now();
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

	if (MULT)
	{
		// Serialize the relinearization (evaluation) key for homomorphic
		// multiplication
		std::ofstream emkeyfile(DATAFOLDER + "/" + "key-eval-mult.txt",
								std::ios::out | std::ios::binary);
		if (emkeyfile.is_open())
		{
			if (cryptoContext->SerializeEvalMultKey(emkeyfile, SerType::BINARY) == false)
			{
				std::cerr << "Error writing serialization of the eval mult keys to "
							 "key-eval-mult.txt"
						  << std::endl;
				return 1;
			}
			emkeyfile.close();
		}
		else
		{
			std::cerr << "Error serializing eval mult keys" << std::endl;
			return 1;
		}
	}
	if (ROT)
	{
		// Serialize the rotation keys
		std::ofstream erkeyfile(DATAFOLDER + "/" + "key-eval-rot.txt",
								std::ios::out | std::ios::binary);
		if (erkeyfile.is_open())
		{
			if (cryptoContext->SerializeEvalAutomorphismKey(erkeyfile, SerType::BINARY) == false)
			{
				std::cerr << "Error writing serialization of the eval rotation keys to "
							 "key-eval-rot.txt"
						  << std::endl;
				return 1;
			}
			erkeyfile.close();
		}
		else
		{
			std::cerr << "Error serializing eval rotation keys" << std::endl;
			return 1;
		}
	}
	std::chrono::steady_clock::time_point cc_ser_end = std::chrono::steady_clock::now();

	// Sample Program: Step 3: Encryption
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

	std::chrono::steady_clock::time_point data_ser_begin = std::chrono::steady_clock::now();
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

	std::chrono::steady_clock::time_point cc_de_begin = std::chrono::steady_clock::now();
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
	if (MULT)
	{
		if (!emkeys.is_open())
		{
			std::cerr << "I cannot read serialization from "
					  << DATAFOLDER + "/key-eval-mult.txt" << std::endl;
			return 1;
		}
		if (cc->DeserializeEvalMultKey(emkeys, SerType::BINARY) == false)
		{
			std::cerr << "Could not deserialize the eval mult key file" << std::endl;
			return 1;
		}
	}
	std::ifstream erkeys(DATAFOLDER + "/key-eval-rot.txt",
						 std::ios::in | std::ios::binary);
	if (ROT)
	{
		if (!erkeys.is_open())
		{
			std::cerr << "I cannot read serialization from "
					  << DATAFOLDER + "/key-eval-rot.txt" << std::endl;
			return 1;
		}
		if (cc->DeserializeEvalAutomorphismKey(erkeys, SerType::BINARY) == false)
		{
			std::cerr << "Could not deserialize the eval rotation key file"
					  << std::endl;
			return 1;
		}
	}
	std::chrono::steady_clock::time_point cc_de_end = std::chrono::steady_clock::now();

	std::chrono::steady_clock::time_point data_de_begin = std::chrono::steady_clock::now();
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
	if (ADD)
	{
		auto ciphertextAdd12 = cc->EvalAdd(ct1, ct2);
		auto ciphertextAddResult = cc->EvalAdd(ciphertextAdd12, ct3);
		cc->Decrypt(sk, ciphertextAddResult, &plaintextAddResult);
	}

	// Homomorphic multiplications
	Plaintext plaintextMultResult;
	if (MULT)
	{
		auto ciphertextMul12 = cc->EvalMult(ct1, ct2);
		auto ciphertextMultResult = cc->EvalMult(ciphertextMul12, ct3);
		cc->Decrypt(sk, ciphertextMultResult, &plaintextMultResult);
	}

	// Homomorphic rotations
	Plaintext plaintextRot1;
	Plaintext plaintextRot2;
	Plaintext plaintextRot3;
	Plaintext plaintextRot4;
	if (ROT)
	{
		auto ciphertextRot1 = cc->EvalAtIndex(ct1, 1);
		auto ciphertextRot2 = cc->EvalAtIndex(ct1, 2);
		auto ciphertextRot3 = cc->EvalAtIndex(ct1, -1);
		auto ciphertextRot4 = cc->EvalAtIndex(ct1, -2);
		cc->Decrypt(sk, ciphertextRot1, &plaintextRot1);
		cc->Decrypt(sk, ciphertextRot2, &plaintextRot2);
		cc->Decrypt(sk, ciphertextRot3, &plaintextRot3);
		cc->Decrypt(sk, ciphertextRot4, &plaintextRot4);
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
	std::cout << "Cipher context serialization: " << std::chrono::duration_cast<std::chrono::microseconds>(cc_ser_end - cc_ser_begin).count() << "[µs]" << std::endl;
	std::cout << "Cipher context deserialization: " << std::chrono::duration_cast<std::chrono::microseconds>(cc_de_end - cc_de_begin).count() << "[µs]" << std::endl;
	std::cout << "Data encryption: " << std::chrono::duration_cast<std::chrono::microseconds>(data_enc_end - data_enc_begin).count() << "[µs]" << std::endl;
	std::cout << "Data serialization: " << std::chrono::duration_cast<std::chrono::microseconds>(data_ser_end - data_ser_begin).count() << "[µs]" << std::endl;
	std::cout << "Data deserialization: " << std::chrono::duration_cast<std::chrono::microseconds>(data_de_end - data_de_begin).count() << "[µs]" << std::endl;
	std::cout << "Operations and decryption: " << std::chrono::duration_cast<std::chrono::microseconds>(hom_end - hom_begin).count() << "[µs]" << std::endl;
	std::cout << "Total: " << std::chrono::duration_cast<std::chrono::microseconds>(tot_end - tot_begin).count() << "[µs]" << std::endl;
	return 0;
}
