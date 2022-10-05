#pragma once

#include <vector>
#include <algorithm> /* std::transform */

#include <openssl/sha.h>

typedef std::vector<unsigned char> bytes;
typedef bytes (*hash_fn)(bytes);
typedef bytes (*prff)(bytes, bytes);

inline bytes itb(int arg)
{
	return bytes({(unsigned char)((arg >> 24) & 0xff),
				  (unsigned char)((arg >> 16) & 0xff),
				  (unsigned char)((arg >> 8) & 0xff),
				  (unsigned char)(arg & 0xff)});
}
inline unsigned int bti(bytes arg)
{
	return (unsigned int)((unsigned char)(arg[0]) << 24 |
						  (unsigned char)(arg[1]) << 16 |
						  (unsigned char)(arg[2]) << 8 |
						  (unsigned char)(arg[3]));
}

inline bytes sha256(bytes arg)
{
	bytes hash(SHA256_DIGEST_LENGTH);
	SHA256_CTX sha256;
	SHA256_Init(&sha256);
	SHA256_Update(&sha256, &arg[0], arg.size());
	SHA256_Final(&hash[0], &sha256);
	return hash;
}
inline bytes hmac_sha256(bytes key, bytes val)
{
	auto vec_xor = [](bytes arg0, unsigned char arg1)
	{
		bytes output;
		std::transform(arg0.begin(),
					   arg0.end(),
					   std::back_inserter(output),
					   [=](unsigned char c)
					   { return c ^ arg1; });
		return output;
	};
	auto vec_cat = [](bytes arg0, bytes arg1)
	{
		bytes output(arg0);
		output.insert(output.end(), arg1.begin(), arg1.end());
		return output;
	};

	return sha256(
		vec_cat(
			vec_xor(
				key,
				0x5c),
			sha256(
				vec_cat(
					vec_xor(
						key,
						0x36),
					val))));
}