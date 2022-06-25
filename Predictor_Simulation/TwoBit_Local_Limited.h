#pragma once
#include <vector>

#include "Predictors.h"

class NBitLocal_limited
{
private:
	int abits;
	std::vector<State> states;
public:
	NBitLocal_limited(int state_bits, int adress_bits);
	~NBitLocal_limited();
	double RunTestSet(std::vector<std::pair<address_t, int>> trace_data);
};