#pragma once
#include <map>
#include <string>
#include <vector>

#include "Predictors.h"

class Local_unlimited
{
private:
	int sbits;
	std::map<address_t, State> states;
public:
	Local_unlimited(int state_bits);
	~Local_unlimited();
	double RunTestSet(std::vector<std::pair<address_t, int>> trace_data);
};

