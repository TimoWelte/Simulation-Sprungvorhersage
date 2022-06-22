#pragma once
#include <string>
#include <vector>
#include "Predictors.h"

class TwoLevelGlobal
{
private:
	History history;
	std::vector<State> states;

public:
	TwoLevelGlobal(int bits);
	TwoLevelGlobal(int history_bits, int state_bits);
	~TwoLevelGlobal();
	double RunTestSet(const std::vector<std::pair<address_t, int>>& trace_data);
	bool Action(bool jump);
};