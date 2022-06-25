#include "Local_unlimited.h"

#include <map>

Local_unlimited::Local_unlimited(int state_bits)
{
	sbits = state_bits;
	states = std::map<address_t, State>();
}

Local_unlimited::~Local_unlimited()
= default;

double Local_unlimited::RunTestSet(std::vector<std::pair<address_t, int>> trace_data)
{
	int correct = 0;

	for(const auto& branch : trace_data)
	{
		const bool is_jump = branch.second;
		const auto index = branch.first;
		auto current = states.find(index);

		if(current == states.end())//not found i
		{
			auto new_state = State(sbits);
			states.insert(std::pair(index, new_state));
			if (!is_jump)correct++;
			new_state.change_state(is_jump);
		}
		else
		{
			if (current->second.is_true() == is_jump)correct++;
			current->second.change_state(is_jump);
		}
	}

	return correct / (double)trace_data.size();
}
