#pragma once
#include <vector>
#include "TwoLevelGlobal.h"

#include "Predictors.h"

class GShare
{
private:
	std::vector<State> states;
	History history;
	int hbits;
public:
	GShare(int HistoryBits, int state_bits);
	~GShare();
	double RunTestSet(const std::vector<std::pair<address_t, int>>& trace_data);
};