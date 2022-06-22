#include "TwoLevelGlobal.h"
#include <vector>

TwoLevelGlobal::TwoLevelGlobal(int HistoryBits)
{
	history = History(HistoryBits);
	states = std::vector<State>(static_cast<int>(pow(2, HistoryBits)));

	for (auto& state : states) state = State(2);
}

TwoLevelGlobal::TwoLevelGlobal(int history_bits, int state_bits)
{
	history = History(history_bits);
	states = std::vector<State>(static_cast<int>(pow(2, history_bits)));

	for (auto& state : states) state = State(state_bits);
}

TwoLevelGlobal::~TwoLevelGlobal() = default;

double TwoLevelGlobal::RunTestSet(const std::vector<std::pair<address_t, int>>& trace_data)
{
	const size_t total = trace_data.size();
	int correct = 0;

	for(const auto& pair : trace_data)
	{
		auto& current_state = states[history.get_index()];
		const bool prediction = current_state.is_true();
		const bool jump = pair.second;

		if (prediction == jump) correct++;
		history.move(jump);
		current_state.change_state(jump);
	}

	return correct / static_cast<double>(total);
}

bool TwoLevelGlobal::Action(bool jump)
{
	auto& current_state = states[history.get_index()];
	const bool prediction = current_state.is_true();
	current_state.change_state(jump);

	history.move(jump);

	return prediction == jump;
}
