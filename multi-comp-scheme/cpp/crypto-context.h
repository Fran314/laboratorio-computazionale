#pragma once

#include "hasher.h"

class CypherContext
{
public:
	void setPrimitives(hash_fn H1,
					   hash_fn H2,
					   prff F);
	void setKeys(int Rprev, int Rsucc);
	int cypher(int data, int t);

private:
	hash_fn H1;
	hash_fn H2;
	prff F;

	int Rprev, Rsucc;
};

void CypherContext::setPrimitives(hash_fn H1,
								  hash_fn H2,
								  prff F)
{
	this->H1 = H1;
	this->H2 = H2;
	this->F = F;
}
void CypherContext::setKeys(int Rprev, int Rsucc)
{
	this->Rprev = Rprev;
	this->Rsucc = Rsucc;
}
int CypherContext::cypher(int data, int t)
{
	unsigned int r_tilde_prev_t = bti(H2(F(H1(itb(Rprev)), itb(t))));
	unsigned int r_tilde_succ_t = bti(H2(F(H1(itb(Rsucc)), itb(t))));
	int r_tilde_t = r_tilde_succ_t - r_tilde_prev_t;

	return data + r_tilde_t;
}