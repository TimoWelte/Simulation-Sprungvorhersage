#include "TwoBit_Local_Limited.h"


NBitLocal_limited::NBitLocal_limited(int state_bits, int adress_bits)
{
	abits = adress_bits;

	if (adress_bits > 27)throw "too much adress bits";
	const int size = (int)pow(2, adress_bits);
	states = std::vector<State>(size);

	for (auto& state : states) state = State(state_bits);
}

NBitLocal_limited::~NBitLocal_limited()
= default;

double NBitLocal_limited::RunTestSet(std::vector<std::pair<address_t, int>> trace_data)
{
	int correct = 0;

	for (auto& i : trace_data)
	{
		const auto& adress = i.first;
		const int is_jump = i.second;

		const auto index = get_index(adress, abits);

		if (is_jump)
		{
			if (states[index].is_true()) correct++;
		}
		else
		{
			if (!states[index].is_true())correct++;
		}
		states[index].change_state(is_jump);
	}

	return correct / (double)trace_data.size();
}
