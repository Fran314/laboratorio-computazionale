#pragma once

#include "hasher.h"

class CypherContext
{
public:
	void setKeys(int Rprev, int Rsucc);
	int cypher(int data, int t);

private:
	bytes Rprev, Rsucc;
};

void CypherContext::setKeys(int Rprev, int Rsucc)
{
	this->Rprev = sha256(itb(Rprev));
	this->Rsucc = sha256(itb(Rsucc));
}
int CypherContext::cypher(int data, int t)
{
	unsigned int r_tilde_prev_t = bti(sha256(hmac_sha256(Rprev, itb(t))));
	unsigned int r_tilde_succ_t = bti(sha256(hmac_sha256(Rsucc, itb(t))));
	int r_tilde_t = r_tilde_succ_t - r_tilde_prev_t;

	return data + r_tilde_t;
}